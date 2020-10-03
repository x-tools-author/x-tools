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

void SAKDebugPageDevice::requestWakeup()
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

void SAKDebugPageDevice::run()
{
    QEventLoop eventLoop;
    QString errorString;
    if (!initializing(errorString)){
        emit deviceStateChanged(false);
        emit messageChanged(errorString, false);
        free();
        return;
    }

    // Open the device
    if (open(errorString)){
        emit deviceStateChanged(true);
        while (true){
            if (isInterruptionRequested()){
                break;
            }

            // Read bytes from device
            QByteArray bytes = read();
            if (bytes.length() > 0){
                emit bytesRead(bytes);
            }

            // Write bytes to device
            while (true) {
                bytes = takeWaitingForWrittingBytes();
                if (bytes.length() > 0){
                    bytes = write(bytes);
                    emit bytesWritten(bytes);
                }else{
                    break;
                }
            }

            // Just for debugging data stream(for test page only)
            bytes = writeForTest();
            if (bytes.length()){
                emit bytesWritten(bytes);
            }

            // Chcked device state
            eventLoop.processEvents();
            if(!checkSomething(errorString)){
                emit messageChanged(errorString, false);
                break;
            }

            if(isInterruptionRequested()){
                break;
            }else{
                // Do something to make cpu happy
                mThreadMutex.lock();
                mThreadWaitCondition.wait(&mThreadMutex, SAK_DEVICE_THREAD_SLEEP_INTERVAL);
                mThreadMutex.unlock();
            }
        }

        close();
    }else{
        emit messageChanged(errorString, false);
    }

    emit deviceStateChanged(false);
    free();
}

bool SAKDebugPageDevice::initializing(QString &errorString)
{
    errorString = QString("Need to override");
    return false;
}

bool SAKDebugPageDevice::open(QString &errorString)
{
    errorString = QString("Need to override");
    return false;
}

QByteArray SAKDebugPageDevice::read()
{
    return QByteArray();
}

QByteArray SAKDebugPageDevice::write(QByteArray bytes)
{
    return bytes;
}

bool SAKDebugPageDevice::checkSomething(QString &errorString)
{
    errorString = QString("Unknow error");
    return true;
}

void SAKDebugPageDevice::close()
{
    // Nothing to do
}

void SAKDebugPageDevice::free()
{
    // Nothing to do
}

QByteArray SAKDebugPageDevice:: writeForTest()
{
    return QByteArray();
}
