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
#include "SAKDebugPageController.hh"

SAKDebugPageController::SAKDebugPageController(QObject *parent)
    :QThread(parent)
{
    // nothing to do
}

SAKDebugPageController::~SAKDebugPageController()
{
    requestInterruption();
    mThreadWaitCondition.wakeAll();
    exit();
    wait();
}

void SAKDebugPageController::wakeMe()
{
    mThreadWaitCondition.wakeAll();
}

void SAKDebugPageController::writeBytes(QByteArray bytes)
{
    mWaitingForWritingBytesListMutex.lock();
    if (bytes.length()){
        mWaitingForWritingBytesList.append(bytes);
    }else{
        mWaitingForWritingBytesList.append(QByteArray("empty"));
    }
    mWaitingForWritingBytesListMutex.unlock();
}

QByteArray SAKDebugPageController::takeWaitingForWrittingBytes()
{
    QByteArray bytes;
    mWaitingForWritingBytesListMutex.lock();
    if (mWaitingForWritingBytesList.length()){
        bytes = mWaitingForWritingBytesList.takeFirst();
    }
    mWaitingForWritingBytesListMutex.unlock();

    return bytes;
}
