/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QEventLoop>
#include "SAKDebugPageDevice.hh"

SAKDebugPageDevice::SAKDebugPageDevice(QObject *parent)
    :QThread(parent)
{
    // nothing to do
}

SAKDebugPageDevice::~SAKDebugPageDevice()
{
    requestInterruption();
    mThreadWaitCondition.wakeAll();
    exit();
    wait();
}

void SAKDebugPageDevice::wakeMe()
{
    mThreadWaitCondition.wakeAll();
}

void SAKDebugPageDevice::writeBytes(QByteArray bytes)
{
    mWaitingForWritingBytesListMutex.lock();
    if (bytes.length()){
        mWaitingForWritingBytesList.append(bytes);
    }else{
        mWaitingForWritingBytesList.append(QByteArray("empty"));
    }
    mWaitingForWritingBytesListMutex.unlock();
}

QByteArray SAKDebugPageDevice::takeWaitingForWrittingBytes()
{
    QByteArray bytes;
    mWaitingForWritingBytesListMutex.lock();
    if (mWaitingForWritingBytesList.length()){
        bytes = mWaitingForWritingBytesList.takeFirst();
    }
    mWaitingForWritingBytesListMutex.unlock();

    return bytes;
}
