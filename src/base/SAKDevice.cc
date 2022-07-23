/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include <QTimer>
#include "SAKDevice.hh"

SAKDevice::SAKDevice(QObject *parent)
    : QThread{parent}
    , SAKObj("SAK.Device")
{

}

void SAKDevice::writeBytes(const QByteArray &bytes)
{
    if (bytes.isEmpty()) {
        printLog(QtInfoMsg,
                 "Can not write an empty array, it will be ignored!");
        return;
    }

    mBytesVectorMutex.lock();
    mBytesVector.append(bytes);
    mBytesVectorMutex.unlock();
}

void SAKDevice::run()
{
    if (!initDevice()) {
        printLog(QtWarningMsg, "Can not initialize device!");
        return;
    }

    QTimer rxTimer;
    initRx(rxTimer);

    QTimer txTimer;
    initTx(txTimer);

    if (mParameterCtx.timingReading) {
        rxTimer.start();
    } else {
        connect(this, &SAKDevice::readyRead, this, [=](){
            QByteArray bytes = readBytesActually();
            if (!bytes.isEmpty()) {
                emit bytesRead(bytes);
            }
        });
    }

    txTimer.start();
    exec();

    uninitDevice();
}

QByteArray SAKDevice::takeBytes()
{
    QByteArray bytes;
    mBytesVectorMutex.lock();
    if (!mBytesVector.isEmpty()){
        bytes = mBytesVector.takeFirst();
    }
    mBytesVectorMutex.unlock();

    return bytes;
}

void SAKDevice::initRx(QTimer &rxTimer)
{
    rxTimer.setInterval(10);
    rxTimer.setSingleShot(true);
    connect(&rxTimer, &QTimer::timeout, &rxTimer, [=, &rxTimer](){
        QByteArray bytes = takeBytes();
        if (!bytes.isEmpty()) {
            if (writeBytesActually(bytes)) {
                emit bytesWritten(bytes);
            }
        }
        rxTimer.start();
    });
}

void SAKDevice::initTx(QTimer &txTimer)
{
    txTimer.setInterval(10);
    txTimer.setSingleShot(true);
    connect(&txTimer, &QTimer::timeout, &txTimer, [=, &txTimer](){
        QByteArray bytes = takeBytes();
        if (!bytes.isEmpty()) {
            if (writeBytesActually(bytes)) {
                emit bytesWritten(bytes);
            }
        }
        txTimer.start();
    });
}
