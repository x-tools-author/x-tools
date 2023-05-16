/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QWebSocket>
#include "SAKWebSocketServerTool.hh"

SAKWebSocketServerTool::SAKWebSocketServerTool(QObject *parent)
    : SAKSocketServerTool{"SAK.WebSocketServerTool", parent}
{

}

bool SAKWebSocketServerTool::initialize()
{
    QString serverName = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    mWebSocketServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mWebSocketServer->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    outputMessage(QtInfoMsg, info);

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
            emit bytesOutputted(bytes, QVariant());
        });

        connect(client, &QWebSocket::binaryMessageReceived, client, [=](const QByteArray &message){
            emit bytesOutputted(message, QVariant());
        });

        connect(client, &QWebSocket::disconnected, client, [=](){
            this->mWebSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });

        connect(client, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
                client, [=](QAbstractSocket::SocketError err){
            Q_UNUSED(err);
            this->mWebSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });
    });

    return true;
}

void SAKWebSocketServerTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    Q_UNUSED(context);
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
            emit bytesInputted(bytes, QVariant());
        }
    }
}

void SAKWebSocketServerTool::uninitialize()
{
    mWebSocketServer->close();
    mWebSocketServer->deleteLater();
    mWebSocketServer = nullptr;
}
