/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QtEndian>
#include <QEventLoop>

#include "SAKCommonDataStructure.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKInputDataFactory.hh"

SAKInputDataFactory::SAKInputDataFactory(QObject *parent)
    :QThread (parent)
    ,mCrcInterface (Q_NULLPTR)
{

}

SAKInputDataFactory::~SAKInputDataFactory()
{
    // Wake the thread to handle temp data
    mThreadCondition.wakeAll();
    // Exit the thread
    requestInterruption();
    mThreadCondition.wakeAll();
    exit();
    wait();
}

void SAKInputDataFactory::cookData(QString rawData, SAKDebugPageInputController::InputParametersContext parameters)
{
    RawDataStruct rawDataStruct;
    rawDataStruct.rawData = rawData;
    rawDataStruct.parameters = parameters;
    mRawDataListMutex.lock();
    mRawDataListMutex.unlock();
    mRawDataList.append(rawDataStruct);
    mThreadCondition.wakeAll();
}

quint32 SAKInputDataFactory::crcCalculate(QByteArray data, int model)
{
    int bitsWidth = mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(model));
    uint8_t crc8;
    uint16_t crc16;
    uint32_t crc32;
    quint32 crc = 0;
    switch (bitsWidth) {
    case 8:
        crc8 = mCrcInterface->crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCommonCrcInterface::CRCModel>(model));
        crc = crc8;
        break;
    case 16:
        crc16 = mCrcInterface->crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCommonCrcInterface::CRCModel>(model));
        crc = crc16;
        break;
    case 32:
        crc32 = mCrcInterface->crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCommonCrcInterface::CRCModel>(model));
        crc = crc32;
        break;
    default:
        break;
    }

    return crc;
}

QByteArray SAKInputDataFactory::rawDataToArray(QString rawData, SAKDebugPageInputController::InputParametersContext parameters)
{
    return SAKCommonDataStructure::stringToByteArray(rawData, static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(parameters.inputModel));
}

QByteArray SAKInputDataFactory::extractCrcData(QByteArray crcData, SAKDebugPageInputController::InputParametersContext parameters)
{
    QByteArray crcInputData;
    int startIndex = parameters.startByte - 1;
    startIndex = startIndex < 0 ? 0 : startIndex;

    int endIndex = (crcData.length() - 1) - (parameters.endByte - 1);
    endIndex = endIndex < 0 ? 0 : endIndex;

    if (((crcData.length() - 1) >= startIndex) && ((crcData.length() - 1) >= endIndex)){
        int length = endIndex - startIndex + 1;
        length = length < 0 ? 0 : length;
        crcInputData = QByteArray(crcData.constData()+startIndex, length);
    }else{
        crcInputData = crcData;
    }

#if 0
    qDebug() << __FUNCTION__ << startIndex << endIndex << QString(crcInputData.toHex(' '));
#endif
    return crcInputData;
}

void SAKInputDataFactory::run()
{
    QEventLoop eventLoop;
    mCrcInterface = new SAKCommonCrcInterface;
    while (true) {
        // Handle input data
        while (true) {
            RawDataStruct rawDataStruct = takeRawData();
            if (rawDataStruct.rawData.length()){
                innnerCookData(rawDataStruct.rawData, rawDataStruct.parameters);
            }else{
                break;
            }
        }

        // emmmm...
        eventLoop.processEvents();

        // Do something make cpu happy
        if (!isInterruptionRequested()){
            mThreadMutex.lock();
            mThreadCondition.wait(&mThreadMutex, 50);
            mThreadMutex.unlock();
        }else{
            break;
        }
    }

    delete mCrcInterface;
    mCrcInterface = Q_NULLPTR;
}

SAKInputDataFactory::RawDataStruct SAKInputDataFactory::takeRawData()
{
    RawDataStruct rawDataStruct;
    mRawDataListMutex.lock();
    if (mRawDataList.length()){
        rawDataStruct = mRawDataList.takeFirst();
    }
    mRawDataListMutex.unlock();

    return rawDataStruct;
}

void SAKInputDataFactory::innnerCookData(QString rawData, SAKDebugPageInputController::InputParametersContext parameters)
{
    QByteArray data = rawDataToArray(rawData, parameters);
    if (parameters.appendCrc){
        // Extract effective crc section
        QByteArray crcInputData = extractCrcData(data, parameters);

        // Calculate the crc value of input data
        uint32_t crc  = crcCalculate(crcInputData, parameters.crcParametersModel);
        uint8_t  crc8  = static_cast<uint8_t>(crc);
        uint16_t crc16 = static_cast<uint16_t>(crc);
        int bitsWidth = mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(parameters.crcParametersModel));
        if (parameters.bigEndian){
            crc16 = qToBigEndian(crc16);
            crc = qToBigEndian(crc);
        }

        // Append crc byte to data
        switch (bitsWidth) {
        case 8:
            data.append(reinterpret_cast<char*>(&crc8), 1);
            break;
        case 16:
            data.append(reinterpret_cast<char*>(&crc16), 2);
            break;
        case 32:
            data.append(reinterpret_cast<char*>(&crc), 4);
            break;
        default:
            break;
        }
    }

    emit dataCooked(data);
}
