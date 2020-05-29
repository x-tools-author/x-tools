/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QtEndian>
#include <QApplication>

#include "SAKGlobal.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"

SAKInputDataFactory::SAKInputDataFactory(QObject *parent)
    :QThread (parent)
    ,crcInterface (Q_NULLPTR)
{
    moveToThread(this);
}

void SAKInputDataFactory:: cookData(QString rawData, SAKDebugPageInputManager::InputParameters parameters)
{
    QByteArray data = rawDataToArray(rawData, parameters);
    if (parameters.addCRC){
        uint32_t crc  = crcCalculate(data, parameters.crcModel);
        uint8_t  crc8  = static_cast<uint8_t>(crc);
        uint16_t crc16 = static_cast<uint16_t>(crc);
        int bitsWidth = crcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(parameters.crcModel));
        if (parameters.bigEndian){
            crc16 = qToBigEndian(crc16);
            crc = qToBigEndian(crc);
        }

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

quint32 SAKInputDataFactory::crcCalculate(QByteArray data, int model)
{
    int bitsWidth = crcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(model));
    uint8_t crc8;
    uint16_t crc16;
    uint32_t crc32;
    quint32 crc = 0;
    switch (bitsWidth) {
    case 8:
        crc8 = crcInterface->crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCRCInterface::CRCModel>(model));
        crc = crc8;
        break;
    case 16:
        crc16 = crcInterface->crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCRCInterface::CRCModel>(model));
        crc = crc16;
        break;
    case 32:
        crc32 = crcInterface->crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCRCInterface::CRCModel>(model));
        crc = crc32;
        break;
    default:
        break;
    }

    return crc;
}

QByteArray SAKInputDataFactory::rawDataToArray(QString rawData, SAKDebugPageInputManager::InputParameters parameters)
{
    QByteArray data;
    if (parameters.inputModel == SAKDataStruct::InputFormatBin){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(Q_NULLPTR, 2)));
        }
    }else if (parameters.inputModel == SAKDataStruct::InputFormatOct){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(Q_NULLPTR, 8)));
        }
    }else if (parameters.inputModel == SAKDataStruct::InputFormatDec){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(Q_NULLPTR, 10)));
        }
    }else if (parameters.inputModel == SAKDataStruct::InputFormatHex){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(Q_NULLPTR, 16)));
        }
    }else if (parameters.inputModel == SAKDataStruct::InputFormatAscii){
        data = rawData.toLatin1();
    }else if (parameters.inputModel == SAKDataStruct::InputFormatUtf8){
        data = rawData.toUtf8();
    }else if (parameters.inputModel == SAKDataStruct::InputFormatUtf8){
        data = rawData.toLocal8Bit();
    }else {
        data = rawData.toLocal8Bit();
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }

    return data;
}

void SAKInputDataFactory::run()
{
    crcInterface = new SAKCRCInterface(this);
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKInputDataFactory::terminate);
    exec();
}
