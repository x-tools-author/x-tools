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
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
{
    // Reconnection
    connect(this, &SAKTcpClientDevice::finished, this, [&](){
        auto parameters = mDeviceController->parameters().value<SAKTcpClientDeviceController::TcpClientParameters>();
        if (parameters.allowAutomaticConnection){
            QTimer *timer = new QTimer(this);
            timer->setInterval(2000);
            timer->setSingleShot(true);
            connect(timer, &QTimer::timeout, this, [=](){
                auto ret = qobject_cast<QTimer*>(sender());
                delete ret;
                start();
            });
            timer->start();
        }
    });
}

bool SAKTcpClientDevice::initializing(QString &errorString)
{
    QEventLoop eventLoop;
    mDeviceController = qobject_cast<SAKTcpClientDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKTcpClientDevice::clientInfoChange, mDeviceController, &SAKTcpClientDeviceController::setClientInfo);
    auto parameters = mDeviceController->parameters().value<SAKTcpClientDeviceController::TcpClientParameters>();
    mLocalHost = parameters.localHost;
    mLocalPort = parameters.localPort;
    mServerHost = parameters.serverHost;
    mServerPort = parameters.serverPort;
    mSpecifyClientAddressAndPort = parameters.specifyClientAddressAndPort;

    mTcpSocket = new QTcpSocket;
    bool bindResult = false;
    if (mSpecifyClientAddressAndPort){
        bindResult = mTcpSocket->bind(QHostAddress(mLocalHost), mLocalPort);
    }else{
        bindResult = mTcpSocket->bind();
    }

    if (!bindResult){
        errorString = tr("Binding failed:") + mTcpSocket->errorString();
        return false;
    }else{
        QString info = mTcpSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mTcpSocket->localPort()));
        emit clientInfoChange(info);
    }

    mTcpSocket->connectToHost(mServerHost, mServerPort);
    if (mTcpSocket->state() != QTcpSocket::ConnectedState){
        if (!mTcpSocket->waitForConnected()){
            errorString = tr("Connect to server failed:") + mTcpSocket->errorString();
            return false;
        }
    }

    return true;
}

bool SAKTcpClientDevice::open(QString &errorString)
{
    if (mTcpSocket->open(QTcpSocket::ReadWrite)){
        errorString = tr("Unknown error");
        return true;
    }else{
        errorString = tr("Can not open device:") + mTcpSocket->errorString();
        return false;
    }
}

QByteArray SAKTcpClientDevice::read()
{
    return mTcpSocket->readAll();
}

QByteArray SAKTcpClientDevice::write(QByteArray bytes)
{
    qint64 ret = mTcpSocket->write(bytes);
    if (ret > 0){
        return bytes;
    }else{
        return QByteArray();
    }
}

bool SAKTcpClientDevice::checkSomething(QString &errorString)
{
    if(mTcpSocket->state() == QTcpSocket::UnconnectedState){
        errorString = tr("Connection has been disconnected.");
        return false;
    }else{
        errorString = tr("Unknown error");
        return true;
    }
}

void SAKTcpClientDevice::close()
{
    if (mTcpSocket->state() == QTcpSocket::ConnectedState){
        mTcpSocket->disconnectFromHost();
    }

    mTcpSocket->close();
    emit clientInfoChange(QString());
}

void SAKTcpClientDevice::free()
{
    delete mTcpSocket;
    mTcpSocket = Q_NULLPTR;
}
