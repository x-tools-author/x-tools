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

#include "SAKSslSocketClientDevice.hh"
#include "SAKSslSocketClientDebugPage.hh"
#include "SAKSslSocketClientDeviceController.hh"

SAKSslSocketClientDevice::SAKSslSocketClientDevice(SAKSslSocketClientDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,mDebugPage (debugPage)
{

}

bool SAKSslSocketClientDevice::initializing(QString &errorString)
{
    QEventLoop eventLoop;
    mDeviceController = qobject_cast<SAKSslSocketClientDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKSslSocketClientDevice::clientInfoChange, mDeviceController, &SAKSslSocketClientDeviceController::setClientInfo);
    auto parameters = mDeviceController->parameters().value<SAKSslSocketClientDeviceController::SslSocketClientParameters>();
    mLocalHost = parameters.localHost;
    mLocalPort = parameters.localPort;
    mServerHost = parameters.serverHost;
    mServerPort = parameters.serverPort;
    mSpecifyClientAddressAndPort = parameters.specifyClientAddressAndPort;

    mSslSocket = new QSslSocket;
    bool bindResult = false;
    if (mSpecifyClientAddressAndPort){
        bindResult = mSslSocket->bind(QHostAddress(mLocalHost), mLocalPort);
    }else{
        bindResult = mSslSocket->bind();
    }

    if (!bindResult){
        errorString = tr("Binding failed:") + mSslSocket->errorString();
        return false;
    }else{
        QString info = mSslSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mSslSocket->localPort()));
        emit clientInfoChange(info);
    }

    mSslSocket->connectToHost(mServerHost, mServerPort);
    if (mSslSocket->state() != QTcpSocket::ConnectedState){
        if (!mSslSocket->waitForConnected()){
            errorString = tr("Connect to server failed:") + mSslSocket->errorString();
            return false;
        }
    }

    return true;
}

bool SAKSslSocketClientDevice::open(QString &errorString)
{
    if (mSslSocket->open(QTcpSocket::ReadWrite)){
        errorString = tr("Unknow error");
        return true;
    }else{
        errorString = tr("Can not open device:") + mSslSocket->errorString();
        return false;
    }
}

QByteArray SAKSslSocketClientDevice::read()
{
    return mSslSocket->readAll();
}

QByteArray SAKSslSocketClientDevice::write(QByteArray bytes)
{
    qint64 ret = mSslSocket->write(bytes);
    if (ret > 0){
        return bytes;
    }else{
        return QByteArray();
    }
}

bool SAKSslSocketClientDevice::checkSomething(QString &errorString)
{
    if(mSslSocket->state() == QTcpSocket::UnconnectedState){
        errorString = tr("Connection has been disconnected.");
        return false;
    }else{
        errorString = tr("Unknow error");
        return true;
    }
}

void SAKSslSocketClientDevice::close()
{
    if (mSslSocket->state() == QTcpSocket::ConnectedState){
        mSslSocket->disconnectFromHost();
    }

    mSslSocket->close();
    emit clientInfoChange(QString());
}

void SAKSslSocketClientDevice::free()
{
    delete mSslSocket;
    mSslSocket = Q_NULLPTR;
}
