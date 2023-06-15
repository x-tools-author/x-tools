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

#include "SAKInterface.hh"
#include "SAKWebSocketServerTool.hh"

SAKWebSocketServerTool::SAKWebSocketServerTool(QObject *parent)
    : SAKSocketServerTool{"SAK.WebSocketServerTool", parent}
{

}

bool SAKWebSocketServerTool::initialize()
{
    QString serverName = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    mWebSocketServer = new QWebSocketServer(serverName,
                                            QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mWebSocketServer->errorString());
        return false;
    }

    QString mBindingIpPort = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    outputMessage(QtInfoMsg, mBindingIpPort);

    connect(mWebSocketServer, &QWebSocketServer::newConnection,
            mWebSocketServer, [=](){
        QWebSocket *client = mWebSocketServer->nextPendingConnection();
        mWebSocketList.append(client);

        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        mClients.append(ipPort);
        emit clientsChanged();

        connect(client, &QWebSocket::textMessageReceived,
                client, [=](const QString &message){
            QByteArray bytes = message.toUtf8();
            QString ip = client->peerAddress().toString();
            quint16 port = client->peerPort();
            QString ipport = QString("%1:%2").arg(ip, QString::number(port));
            QString hex = QString::fromUtf8(bytes);
            outputMessage(QtInfoMsg, QString("%1<-%2:%3")
                                         .arg(mBindingIpPort, ipport, hex));
            emit bytesOutputted(bytes, QVariant());
        });

        connect(client, &QWebSocket::binaryMessageReceived,
                client, [=](const QByteArray &message){
            QString ip = client->peerAddress().toString();
            quint16 port = client->peerPort();
            QString ipport = QString("%1:%2").arg(ip, QString::number(port));
            QString hex = QString::fromUtf8(message);
            outputMessage(QtInfoMsg, QString("%1<-%2:%3")
                                         .arg(mBindingIpPort, ipport, hex));
            emit bytesOutputted(message, QVariant());
        });

        connect(client, &QWebSocket::disconnected, client, [=](){
            this->mWebSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            outputMessage(QtInfoMsg, QString("Connection(%1) disconnected:%2")
                                         .arg(mBindingIpPort,
                                              client->errorString()));
            emit clientsChanged();
        });

        connect(client, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                client, [=](QAbstractSocket::SocketError err){
            Q_UNUSED(err);
            this->mWebSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            outputMessage(QtInfoMsg, QString("Error occurred:%1")
                                         .arg(client->errorString()));
            emit clientsChanged();
        });
    });

    return true;
}

void SAKWebSocketServerTool::writeBytes(const QByteArray &bytes,
                                        const QVariant &context)
{
    Q_UNUSED(context);
    if (mClientIndex >= 0 && mClientIndex < mWebSocketList.length()) {
        QWebSocket *client = mWebSocketList.at(mClientIndex);
        writeBytesInner(client, bytes, context);
    } else {
        for (auto client : mWebSocketList) {
            writeBytesInner(client, bytes, context);
        }
    }
}

void SAKWebSocketServerTool::uninitialize()
{
    mWebSocketServer->close();
    mWebSocketServer->deleteLater();
    mWebSocketServer = nullptr;
}

void SAKWebSocketServerTool::writeBytesInner(QWebSocket *client,
                                             const QByteArray &bytes,
                                             const QVariant &context)
{
    qint64 ret = -1;
    QString hex;
    if (mMessageType == 0) {
        hex = QString::fromLatin1(SAKInterface::arrayToHex(bytes, ' '));
        ret = client->sendBinaryMessage(bytes);
    } else {
        hex = QString::fromUtf8(bytes);
        ret = client->sendTextMessage(QString::fromUtf8(bytes));
    }

    if (ret == -1) {
        outputMessage(QtWarningMsg, client->errorString());
    } else {
        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipport = QString("%1:%2").arg(ip, QString::number(port));
        outputMessage(QtInfoMsg, QString("%1->%2:%3")
                                     .arg(this->mBindingIpPort, ipport, hex));
        emit bytesInputted(bytes, context);
    }
}
