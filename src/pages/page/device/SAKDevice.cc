/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKDevice.hh"

SAKDevice::SAKDevice(QObject *parent)
    :QThread(parent)
{

}

SAKDevice::~SAKDevice()
{

}

void SAKDevice::wakeMe()
{
    threadWaitCondition.wakeAll();
}

void SAKDevice::writeBytes(QByteArray bytes)
{
    waitingForWritingBytesListMutex.lock();
    if (bytes.length()){
        waitingForWritingBytesList.append(bytes);
    }else{
        waitingForWritingBytesList.append(QByteArray("empty"));
    }
    waitingForWritingBytesListMutex.unlock();
}

QByteArray SAKDevice::takeWaitingForWrittingBytes()
{
    QByteArray bytes;
    waitingForWritingBytesListMutex.lock();
    if (waitingForWritingBytesList.length()){
        bytes = waitingForWritingBytesList.takeFirst();
    }else{
        bytes = QByteArray("");
    }
    waitingForWritingBytesListMutex.unlock();

    return bytes;
}
