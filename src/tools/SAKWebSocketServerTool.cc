/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QWebSocket>
#include "EDWebSocketServerTool.hpp"

EDWebSocketServerTool::EDWebSocketServerTool(QObject *parent)
    : EDBaseTool{"ED.WebSocketServerTool", parent}
{

}

bool EDWebSocketServerTool::initialize(QString &errStr)
{
    QString serverName = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    mWebSocketServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mWebSocketServer->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    outputMessage(QtInfoMsg, info);
    setToolFlag(info);

    connect(mWebSocketServer, &QWebSocketServer::newConnection, mWebSocketServer, [=](){
        QWebSocket *client = mWebSocketServer->nextPendingConnection();
        mWebSocketList.append(client);

        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        mClients.append(ipPort);
        emit clientsChanged();

        connect(client, &QWebSocket::textMessageReceived, client, [=](const QString &message){
            QByteArray bytes = message.toUtf8();
            emit bytesOutputted(ipPort, bytes);
        });

        connect(client, &QWebSocket::binaryMessageReceived, client, [=](const QByteArray &message){
            emit bytesOutputted(ipPort, message);
        });

        connect(client, &QWebSocket::disconnected, client, [=](){
            this->mWebSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });

        connect(client, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), client, [=](QAbstractSocket::SocketError err){
            this->mWebSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });
    });

    return true;
}

void EDWebSocketServerTool::inputBytesHandler(const QByteArray &bytes)
{
    if (mClientIndex >= 0 && mClientIndex < mWebSocketList.length()) {
        QWebSocket *socket = mWebSocketList.at(mClientIndex);
        qint64 ret = -1;
        if (mMessageType == 0) {
            ret = socket->sendBinaryMessage(bytes);
        } else {
            ret = socket->sendTextMessage(QString::fromUtf8(bytes));
        }

        if (ret == -1) {
            outputMessage(QtWarningMsg, socket->errorString());
        } else {
            emit bytesInputted(toolFlag(), bytes);
        }
    }
}

void EDWebSocketServerTool::outputBytesHandler()
{
    // Nothing to do yet.
}

void EDWebSocketServerTool::uninitialize()
{
    mWebSocketServer->close();
    mWebSocketServer->deleteLater();
    mWebSocketServer = nullptr;
}
