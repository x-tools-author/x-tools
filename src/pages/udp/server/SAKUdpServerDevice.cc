/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugPage.hh"
#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugPage.hh"
#include "SAKUdpServerDeviceController.hh"

SAKUdpServerDevice::SAKUdpServerDevice(SAKUdpServerDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,mDebugPage(debugPage)
    ,mUdpServer(Q_NULLPTR)
{

}

void SAKUdpServerDevice::run()
{
    QEventLoop eventLoop;
    SAKUdpServerDeviceController *deviceController = mDebugPage->controllerInstance();
    mServerHost = deviceController->serverHost();
    mServerPort = deviceController->serverPort();

    QList<QTcpSocket*> clientList;
    mUdpServer = new QUdpSocket;
    if (!mUdpServer->bind(QHostAddress(mServerHost), mServerPort)){
        emit deviceStateChanged(false);
        emit messageChanged(tr("Binding failed：")+mUdpServer->errorString(), false);
        return;
    }

    if (mUdpServer->open(QUdpSocket::ReadWrite)){
        emit deviceStateChanged(true);
        while (true){
            // The interface muse be called, or can not to read datagram.
            eventLoop.processEvents();
            // Read data
            innerReadBytes(deviceController);
            // Write data
            innerWriteBytes(deviceController);

            // Do something make cpu happy
            if (isInterruptionRequested()){
                break;
            }else{
                mThreadMutex.lock();
                mThreadWaitCondition.wait(&mThreadMutex, SAK_DEVICE_THREAD_SLEEP_INTERVAL);
                mThreadMutex.unlock();
            }
        }
    }else{
        emit deviceStateChanged(false);
        emit messageChanged(tr("Open device failed：") + mUdpServer->errorString(), false);
    }

    mUdpServer->close();
    delete mUdpServer;
    emit deviceStateChanged(false);
}

void SAKUdpServerDevice::innerReadBytes(SAKUdpServerDeviceController *deviceController)
{
    while (mUdpServer->hasPendingDatagrams()) {
        qint64 size = mUdpServer->pendingDatagramSize();
        QByteArray bytes;
        bytes.resize(size);
        QHostAddress peerAddress;
        quint16 peerPort;
        qint64 ret = mUdpServer->readDatagram(bytes.data(), size, &peerAddress, &peerPort);
        if (ret > 0){
            QString currentHost = deviceController->currentClientHost();
            quint16 currentPort = deviceController->currentClientPort();

            // Do not ignore the first frame.
            if (deviceController->hasNoClient()){
                deviceController->addClientSafely(peerAddress.toString(), peerPort);
                emit bytesRead(bytes);
            }else{
                deviceController->addClientSafely(peerAddress.toString(), peerPort);
                if ((currentHost == peerAddress.toString()) && (currentPort == peerPort)){
                    emit bytesRead(bytes);
                }
            }
        }
    }
}

void SAKUdpServerDevice::innerWriteBytes(SAKUdpServerDeviceController *deviceController)
{
    while (true){
        QByteArray bytes = takeWaitingForWrittingBytes();
        if (bytes.length()){
            QString currentHost = deviceController->currentClientHost();
            quint16 currentPort = deviceController->currentClientPort();
            qint64 ret = mUdpServer->writeDatagram(bytes, QHostAddress(currentHost), currentPort);
            if (ret > 0){
                emit bytesWritten(bytes);
            }else{
                emit messageChanged(QString("%1: %2").arg(tr("Write data error")).arg(mUdpServer->errorString()), false);
            }
        }else{
            break;
        }
    }
}
