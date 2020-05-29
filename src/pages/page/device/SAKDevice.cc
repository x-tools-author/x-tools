/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
