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
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
    ,mUdpServer(Q_NULLPTR)
{

}

bool SAKUdpServerDevice::initializing(QString &errorString)
{
    mDeviceController = qobject_cast<SAKUdpServerDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKUdpServerDevice::addClient, mDeviceController, &SAKUdpServerDeviceController::addClient);
    auto parameters = mDeviceController->parameters().value<SAKUdpServerDeviceController::UdpServerParameters>();

    mUdpServer = new QUdpSocket;
    if (!mUdpServer->bind(QHostAddress(parameters.serverHost), parameters.serverPort, QUdpSocket::ShareAddress)){
        errorString = tr("Binding failed：") + mUdpServer->errorString();
        return false;
    }

    return true;
}

bool SAKUdpServerDevice::open(QString &errorString)
{
    if (mUdpServer->open(QUdpSocket::ReadWrite)){
        errorString = tr("Unknown error");
        return true;
    }else{
        errorString = tr("Open device failed:") + mUdpServer->errorString();
        return false;
    }
}

QByteArray SAKUdpServerDevice::read()
{
    while (mUdpServer->hasPendingDatagrams()) {
        qint64 size = mUdpServer->pendingDatagramSize();
        QByteArray bytes;
        bytes.resize(size);
        QHostAddress peerAddress;
        quint16 peerPort;
        qint64 ret = mUdpServer->readDatagram(bytes.data(), size, &peerAddress, &peerPort);
        if (ret > 0){
            auto parameters = mDeviceController->parameters().value<SAKUdpServerDeviceController::UdpServerParameters>();
            QString currentHost = parameters.currentClientHost;
            quint16 currentPort = parameters.currentClientPort;

            if (currentHost.isEmpty()){
                emit addClient(peerAddress.toString(), peerPort);
                emit bytesRead(bytes);
            }else{
                QStringList clients = parameters.clients;
                QString client = QString("%1:%2").arg(peerAddress.toString()).arg(QString::number(peerPort));
                if (!clients.contains(client)){
                    emit addClient(peerAddress.toString(), peerPort);
                }

                if ((currentHost == peerAddress.toString()) && (currentPort == peerPort)){
                    emit bytesRead(bytes);
                }
            }
        }
    }

    return QByteArray();
}

QByteArray SAKUdpServerDevice::write(QByteArray bytes)
{
    auto parameters = mDeviceController->parameters().value<SAKUdpServerDeviceController::UdpServerParameters>();
    QString currentHost = parameters.currentClientHost;
    quint16 currentPort = parameters.currentClientPort;
    qint64 ret = mUdpServer->writeDatagram(bytes, QHostAddress(currentHost), currentPort);
    if (ret > 0){
        return bytes;
    }

    return QByteArray();
}

bool SAKUdpServerDevice::checkSomething(QString &errorString)
{
    errorString = tr("Unknown error");
    return true;
}

void SAKUdpServerDevice::close()
{
    mUdpServer->close();
}

void SAKUdpServerDevice::free()
{
    delete mUdpServer;
    mUdpServer = Q_NULLPTR;
}
