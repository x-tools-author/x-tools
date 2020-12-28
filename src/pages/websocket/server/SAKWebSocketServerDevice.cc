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
#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketServerDevice.hh"
#include "SAKWebSocketServerDebugPage.hh"
#include "SAKWebSocketServerDeviceController.hh"

SAKWebSocketServerDevice::SAKWebSocketServerDevice(SAKWebSocketServerDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mWebSocketServer(Q_NULLPTR)
    ,mDebugPage(debugPage)
{

}

bool SAKWebSocketServerDevice::initializing(QString &errorString)
{
    mDeviceController = qobject_cast<SAKWebSocketServerDeviceController*>(mDebugPage->deviceController());
    auto parameters = mDeviceController->parameters().value<SAKWebSocketServerDeviceController::WebSocketServerParameters>();

    connect(this, &SAKWebSocketServerDevice::addClient, mDeviceController, &SAKWebSocketServerDeviceController::addClient);
    connect(this, &SAKWebSocketServerDevice::removeClient, mDeviceController, &SAKWebSocketServerDeviceController::removeClient);
    connect(this, &SAKWebSocketServerDevice::clearClient, mDeviceController, &SAKWebSocketServerDeviceController::clearClient);

    QString serverName = QString("%1:%2").arg(parameters.serverHost).arg(parameters.serverPort);
    mWebSocketServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(parameters.serverHost), parameters.serverPort)){
        errorString = tr("Listen failed:") + mWebSocketServer->errorString();
        return false;
    }

    return true;
}

bool SAKWebSocketServerDevice::open(QString &errorString)
{
    errorString = tr("Unknown error");
    return true;
}

QByteArray SAKWebSocketServerDevice::read()
{
    return QByteArray();
}

QByteArray SAKWebSocketServerDevice::write(QByteArray bytes)
{
    auto parameters = mDeviceController->parameters().value<SAKWebSocketServerDeviceController::WebSocketServerParameters>();
    for (auto &var : mClientList){
        QString peerHost = var->peerAddress().toString();
        quint16 peerPort = var->peerPort();

        if ((parameters.currentClientHost == peerHost) && (parameters.currentClientPort == peerPort)){
            qint64 ret = 0;
            if (parameters.sendingType == SAKCommonDataStructure::WebSocketSendingTypeText){
                ret = var->sendTextMessage(QString(bytes));
            }else{
                ret = var->sendBinaryMessage(bytes);
            }

            if (ret > 0){
                return bytes;
            }
        }
    }

    return QByteArray();
}

bool SAKWebSocketServerDevice::checkSomething(QString &errorString)
{
    while (mWebSocketServer->hasPendingConnections()){
        QWebSocket *socket = mWebSocketServer->nextPendingConnection();
        if (socket){
            mClientList.append(socket);
            emit addClient(socket->peerAddress().toString(), socket->peerPort(), socket);

            connect(socket, &QWebSocket::textMessageReceived, [=](const QString message){
                readBytesActually(socket, message.toUtf8());
            });

            connect(socket, &QWebSocket::binaryMessageReceived, [=](const QByteArray message){
                readBytesActually(socket, message);
            });

            // Remove the socket which is offline(It seems to be not effective)
            connect(socket, &QWebSocket::disconnected, [&](){
                emit removeClient(socket);
                mClientList.removeOne(socket);
            });
        }
    }

    QList<QWebSocket*> needTobeDeleteSocketList;
    for (auto &var : mClientList){
        if (var->state() == QAbstractSocket::UnconnectedState){
            needTobeDeleteSocketList.append(var);
        }
    }

    for (auto &var : needTobeDeleteSocketList){
        emit removeClient(var);
    }

    while (needTobeDeleteSocketList.count()) {
        auto var = needTobeDeleteSocketList.takeFirst();
        mClientList.removeOne(var);
        var->deleteLater();
    }

    errorString = tr("Unknown error");
    return true;
}

void SAKWebSocketServerDevice::close()
{
    mWebSocketServer->close();
    emit clearClient();
}

void SAKWebSocketServerDevice::free()
{
    delete mWebSocketServer;
    mWebSocketServer = Q_NULLPTR;
}

void SAKWebSocketServerDevice::readBytesActually(QWebSocket *socket, QByteArray bytes)
{
    auto parameters = mDeviceController->parameters().value<SAKWebSocketServerDeviceController::WebSocketServerParameters>();
    QString currentClientHost = parameters.currentClientHost;
    QString peerHost = socket->peerAddress().toString();
    quint16 currentClientPort = parameters.currentClientPort;
    quint16 peerPort = socket->peerPort();

    if (bytes.length()){
        if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
            emit bytesRead(bytes);
        }
    }
}
