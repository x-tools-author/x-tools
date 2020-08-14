/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDevice::SAKTcpClientDevice(SAKTcpClientDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,debugPage (debugPage)
{

}

bool SAKTcpClientDevice::initializing(QString &errorString)
{
    QEventLoop eventLoop;
    SAKTcpClientDeviceController *deviceController = qobject_cast<SAKTcpClientDeviceController*>(debugPage->deviceController());
    auto parameters = deviceController->parameters().value<SAKTcpClientDeviceController::TcpClientParameters>();
    localHost = parameters.localHost;
    localPort = parameters.localPort;
    serverHost = parameters.serverHost;
    serverPort = parameters.serverPort;
    specifyClientAddressAndPort = parameters.specifyClientAddressAndPort;

    tcpSocket = new QTcpSocket;
    bool bindResult = false;
    if (specifyClientAddressAndPort){
        bindResult = tcpSocket->bind(QHostAddress(localHost), localPort);
    }else{
        bindResult = tcpSocket->bind();
    }

    if (!bindResult){
        errorString = tr("Binding failed:") + tcpSocket->errorString();
        return false;
    }

    tcpSocket->connectToHost(serverHost, serverPort);
    if (tcpSocket->state() != QTcpSocket::ConnectedState){
        if (!tcpSocket->waitForConnected()){
            errorString = tr("Connect to server failed:")+tcpSocket->errorString();
            return false;
        }
    }

    return true;
}

bool SAKTcpClientDevice::open(QString &errorString)
{
    if (tcpSocket->open(QTcpSocket::ReadWrite)){
        errorString = tr("Unknow error.");
        return true;
    }else{
        errorString = tr("Can not open device:") + tcpSocket->errorString();
        return false;
    }
}

QByteArray SAKTcpClientDevice::read()
{
    return tcpSocket->readAll();
}

QByteArray SAKTcpClientDevice::write(QByteArray bytes)
{
    qint64 ret = tcpSocket->write(bytes);
    if (ret > 0){
        return bytes;
    }else{
        return QByteArray();
    }
}

bool SAKTcpClientDevice::checkSomething(QString &errorString)
{
    if(tcpSocket->state() == QTcpSocket::UnconnectedState){
        errorString = tr("Connection has been disconnected.");
        return false;
    }else{
        errorString = tr("Unknow error.");
        return true;
    }
}

void SAKTcpClientDevice::close()
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState){
        tcpSocket->disconnectFromHost();
        tcpSocket->waitForDisconnected();
    }
    tcpSocket->close();
}

void SAKTcpClientDevice::free()
{
    delete tcpSocket;
}
