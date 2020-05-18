/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QtEndian>

#include "SAKCRCInterface.hh"
#include "SAKDebuggerTextInput.hh"
#include "SAKDebuggerInputSettings.hh"
#include "SAKDebuggerInputTextFactory.hh"

SAKDebuggerInputTextFactory::SAKDebuggerInputTextFactory(QObject *parent)
    :QThread(parent)
{

}

SAKDebuggerInputTextFactory::~SAKDebuggerInputTextFactory()
{

}

void SAKDebuggerInputTextFactory::addRawData(QString rawData)
{
    rawDataListMutex.lock();
    rawDataList.append(rawData);
    rawDataListMutex.unlock();

    wakeMe();
}

void SAKDebuggerInputTextFactory::wakeMe()
{
    threadWaitCondition.wakeAll();
}

void SAKDebuggerInputTextFactory::setInputSettings(SAKDebuggerInputSettings *settings)
{
    inputSettings = settings;
}

void SAKDebuggerInputTextFactory::setTextInput(SAKDebuggerTextInput *input)
{
    textInput = input;
}

void SAKDebuggerInputTextFactory::run()
{
    while (1) {
        /// @brief 主动退出线程
        if (isInterruptionRequested()){
            break;
        }

        while (1) {
            /// @brief 主动退出线程
            if (isInterruptionRequested()){
                break;
            }

            /// @brief 提取数据
            QString rawData = takeRawData();
            if (rawData.isEmpty()){
                break;
            }

            /// @brief 处理数据
            int format = inputSettings->paraCurrentInputFromat();
            QByteArray cookedData = cookData(rawData, format);

            /// @brief crc处理
            bool addFlag = textInput->paraAddCRCFlag();
            if (addFlag){
                bool bigEndian = textInput->paraBigEndianCRCFlag();
                int model = inputSettings->paraCurrentCRCModel();
                QByteArray crc = crcCalculate(cookedData, bigEndian, model);
                cookedData.append(crc);
            }

            /// @brief 发送数据
            emit dataCooked(cookedData);
        }

        /// @brief 等待50毫秒唤醒一次（阻塞过程中外部可以唤醒该程序）
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, 50);
        threadMutex.unlock();
    }
}

QString SAKDebuggerInputTextFactory::takeRawData()
{
    rawDataListMutex.lock();
    QString str("");
    if (rawDataList.length()){
        str = rawDataList.takeFirst();
    }
    rawDataListMutex.unlock();

    return str;
}

QByteArray SAKDebuggerInputTextFactory::cookData(QString rawData, int format)
{
    QByteArray cookedData;
    if (format == SAKDebuggerInputSettings::Bin){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            cookedData.append(static_cast<int8_t>(QString(str).toInt(nullptr, 2)));
        }
    }else if (format == SAKDebuggerInputSettings::Otc){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            cookedData.append(static_cast<int8_t>(QString(str).toInt(nullptr, 8)));
        }
    }else if (format == SAKDebuggerInputSettings::Dec){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            cookedData.append(static_cast<int8_t>(QString(str).toInt(nullptr, 10)));
        }
    }else if (format == SAKDebuggerInputSettings::Hex){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            cookedData.append(static_cast<int8_t>(QString(str).toInt(nullptr, 16)));
        }
    }else if (format == SAKDebuggerInputSettings::Ascii){
        cookedData = rawData.toLatin1();
    }else if (format == SAKDebuggerInputSettings::Utf8){
        cookedData = rawData.toUtf8();
    }else if (format == SAKDebuggerInputSettings::System){
        cookedData = rawData.toLocal8Bit();
    }else {
        cookedData = rawData.toLocal8Bit();
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }

    return cookedData;
}

QByteArray SAKDebuggerInputTextFactory::crcCalculate(QByteArray input, bool bigEndian, int model)
{
    SAKCRCInterface crcInterface;
    int bitsWidth = crcInterface.getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(model));
    uint8_t crc8;
    uint16_t crc16;
    uint32_t crc32;
    QByteArray crc = 0;
    switch (bitsWidth) {
    case 8:
        crc8 = crcInterface.crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(input.data()), static_cast<quint64>(input.length()), static_cast<SAKCRCInterface::CRCModel>(model));
        crc.append(reinterpret_cast<char*>(&crc8), 1);
        break;
    case 16:
        crc16 = crcInterface.crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(input.data()), static_cast<quint64>(input.length()), static_cast<SAKCRCInterface::CRCModel>(model));
        crc16 = bigEndian ? qToBigEndian<uint16_t>(crc16) : crc16;
        crc.append(reinterpret_cast<char*>(&crc16), 2);
        break;
    case 32:
        crc32 = crcInterface.crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(input.data()), static_cast<quint64>(input.length()), static_cast<SAKCRCInterface::CRCModel>(model));
        crc32 = bigEndian ? qToBigEndian<uint32_t>(crc32) : crc32;
        crc.append(reinterpret_cast<char*>(&crc32), 4);
        break;
    default:
        break;
    }

    return crc;
}
