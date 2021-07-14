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
#include <QWebSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketClientDevice.hh"
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketClientDeviceController.hh"

SAKWebSocketClientDevice::SAKWebSocketClientDevice(SAKWebSocketClientDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage (debugPage)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}

bool SAKWebSocketClientDevice::initializing(QString &errorString)
{
    mController = qobject_cast<SAKWebSocketClientDeviceController*>(mDebugPage->deviceController());
    mWebSocket = new QWebSocket;
    connect(this, &SAKWebSocketClientDevice::clientInfoChanged, mController, &SAKWebSocketClientDeviceController::setClientInfo);

    connect(mWebSocket, &QWebSocket::connected, [=](){
        QString info = mWebSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mWebSocket->localPort()));
        emit clientInfoChanged(info);
    });

    connect(mWebSocket, &QWebSocket::disconnected, [=](){
        emit clientInfoChanged(QString());
    });

    connect(mWebSocket, &QWebSocket::binaryFrameReceived, [=](QByteArray message){
        emit bytesRead(message);
    });

    connect(mWebSocket, &QWebSocket::textMessageReceived, [=](QString message){
        emit bytesRead(message.toUtf8());
    });

    errorString = tr("Unknown error");
    return true;
}

bool SAKWebSocketClientDevice::open(QString &errorString)
{
    auto parameters = mController->parameters().value<SAKWebSocketClientDeviceController::WebSocketClientParameters>();
    mWebSocket->open(parameters.serverAddress);
    errorString = tr("Unknown error");
    return true;
}

QByteArray SAKWebSocketClientDevice::read()
{
    return QByteArray();
}

QByteArray SAKWebSocketClientDevice::write(QByteArray bytes)
{
    if (mWebSocket->state() == QAbstractSocket::ConnectedState){
        qint64 ret = 0;
        auto parameters = mController->parameters().value<SAKWebSocketClientDeviceController::WebSocketClientParameters>();
        if (parameters.sendingType == SAKCommonDataStructure::WebSocketSendingTypeText){
            ret = mWebSocket->sendTextMessage(QString(bytes));
        }else{
            ret = mWebSocket->sendBinaryMessage(bytes);
        }

        if (ret > 0){
            return bytes;
        }
    }

    return QByteArray();
}

bool SAKWebSocketClientDevice::checkSomething(QString &errorString)
{
    if (mWebSocket->state() == QAbstractSocket::UnconnectedState){
        errorString = tr("Connection has been disconnect.");
        return false;
    }

    errorString = tr("Unknown error");
    return true;
}

void SAKWebSocketClientDevice::close()
{
    mWebSocket->close();
}

void SAKWebSocketClientDevice::free()
{
    mWebSocket->deleteLater();
    mWebSocket = Q_NULLPTR;
}
