/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QtEndian>
#include <QApplication>

#include "SAKBase.hh"
#include "SAKCRCInterface.hh"
#include "InputDataFactory.hh"

InputDataFactory::InputDataFactory(QObject *parent)
    :QThread (parent)
    ,crcInterface (nullptr)
{
    moveToThread(this);
}

void InputDataFactory:: cookData(QString rawData, DebugPageInputManager::InputParameters parameters)
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

quint32 InputDataFactory::crcCalculate(QByteArray data, int model)
{
    int bitsWidth = crcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(model));
    uint8_t crc8;
    uint16_t crc16;
    uint32_t crc32;
    quint32 crc = 0;
    switch (bitsWidth) {
    case 8:
        crcInterface->crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), crc8, static_cast<SAKCRCInterface::CRCModel>(model));
        crc = crc8;
        break;
    case 16:
        crcInterface->crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), crc16, static_cast<SAKCRCInterface::CRCModel>(model));
        crc = crc16;
        break;
    case 32:
        crcInterface->crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), crc32, static_cast<SAKCRCInterface::CRCModel>(model));
        crc = crc32;
        break;
    default:
        break;
    }

    return crc;
}

QByteArray InputDataFactory::rawDataToArray(QString rawData, DebugPageInputManager::InputParameters parameters)
{
    QByteArray data;
    if (parameters.inputModel == SAKBase::Bin){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 2)));
        }
    }else if (parameters.inputModel == SAKBase::Oct){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 8)));
        }
    }else if (parameters.inputModel == SAKBase::Dec){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 10)));
        }
    }else if (parameters.inputModel == SAKBase::Hex){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 16)));
        }
    }else if (parameters.inputModel == SAKBase::Ascii){
        data = rawData.toLatin1();
    }else if (parameters.inputModel == SAKBase::Local){
        data = rawData.toLocal8Bit();
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }

    return data;
}

void InputDataFactory::run()
{
    crcInterface = new SAKCRCInterface(this);
    connect(qApp, &QApplication::lastWindowClosed, this, &InputDataFactory::terminate);
    exec();
}
