/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QEventLoop>

#include "SAKGlobal.hh"
#include "SAKDataStruct.hh"
#include "SAKOutputDataFactory.hh"

SAKOutputDataFactory::SAKOutputDataFactory(QObject *parent)
    :QThread (parent)
{

}

SAKOutputDataFactory::~SAKOutputDataFactory()
{
    requestInterruption();
    threadWaitCondition.wakeAll();
    wait();
}

void SAKOutputDataFactory::run()
{
    QEventLoop eventLoop;
    while (true) {
        /// @brief 响应外部中断请求
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 处理数据
        while (true) {
            RawDataStruct rawData = takeRawData();
            if (rawData.rawData.length()){
                innerCookData(rawData.rawData, rawData.parameters);
            }else{
                break;
            }
        }

        /// @brief 处理事件循环
        eventLoop.processEvents();

        /// @brief 线程睡眠
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, 50);
        threadMutex.unlock();
    }
}

void SAKOutputDataFactory::innerCookData(QByteArray rawData, SAKDebugPageOutputManager::OutputParameters parameters)
{
    QString str;

    str.append("<font color=silver>[</font>");

    if (parameters.showDate){
        str.append(QDate::currentDate().toString("yyyy-MM-dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.showTime){
        if (parameters.showMS){
            str.append(QTime::currentTime().toString("hh:mm:ss.zzz "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.isReceivedData){
        str.append("<font color=red>Rx</font>");
    }else {
        str.append("<font color=blue>Tx</font>");
    }
    str.append("<font color=silver>] </font>");

    if (parameters.textModel == SAKDataStruct::OutputFormatBin){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 2), 8, '0'));
        }
    }else if (parameters.textModel == SAKDataStruct::OutputFormatOct){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 8), 3, '0'));
        }
    }else if (parameters.textModel == SAKDataStruct::OutputFormatDec){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 10)));
        }
    }else if (parameters.textModel == SAKDataStruct::OutputFormatHex){
        for (int i = 0; i < rawData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(rawData.at(i)), 16), 2, '0'));
        }
    }else if (parameters.textModel == SAKDataStruct::OutputFormatAscii){
        str.append(QString::fromLatin1(rawData));
    }else if (parameters.textModel == SAKDataStruct::OutputFormatUtf8){
        str.append(QString::fromUtf8(rawData));
    }else if (parameters.textModel == SAKDataStruct::OutputFormatUtf16){
        str.append(QString::fromUtf16(reinterpret_cast<const ushort*>(rawData.constData()),rawData.length()));
    }else if (parameters.textModel == SAKDataStruct::OutputFormatUcs4){
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t*>(rawData.constData()),rawData.length()));
    }else if (parameters.textModel == SAKDataStruct::OutputFormatStdwstring){
        str.append(QString::fromWCharArray(reinterpret_cast<const wchar_t*>(rawData.constData()),rawData.length()));
    }else if (parameters.textModel == SAKDataStruct::OutputFormatLocal){
        str.append(QString::fromLocal8Bit(rawData));
    }else {
        str.append(QString::fromUtf8(rawData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
    }

    emit dataCooked(str);
}

SAKOutputDataFactory::RawDataStruct SAKOutputDataFactory::takeRawData()
{
    RawDataStruct rawData;
    rawDataListMutex.lock();
    if (rawDataList.length()){
        rawData = rawDataList.takeFirst();
    }
    rawDataListMutex.unlock();

    return rawData;
}

void SAKOutputDataFactory::cookData(QByteArray rawData, SAKDebugPageOutputManager::OutputParameters parameters)
{
    RawDataStruct rawDataStruct;
    rawDataStruct.rawData = rawData;
    rawDataStruct.parameters = parameters;

    rawDataListMutex.lock();
    rawDataList.append(rawDataStruct);
    rawDataListMutex.unlock();

    threadWaitCondition.wakeAll();
}
