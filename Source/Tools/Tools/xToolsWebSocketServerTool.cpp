/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsWebSocketServerTool.h"

#include <QWebSocket>

#include "xToolsCompatibility.h"

xToolsWebSocketServerTool::xToolsWebSocketServerTool(QObject *parent)
    : xToolsSocketServerTool{parent}
{}

bool xToolsWebSocketServerTool::initialize(QString &errStr)
{
    QString serverName = QString("%1:%2").arg(m_serverIp).arg(m_serverPort);
    mWebSocketServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(m_serverIp), m_serverPort)) {
        errStr = mWebSocketServer->errorString();
        qWarning() << errStr;
        return false;
    }

    QString mBindingIpPort = QString("%1:%2").arg(m_serverIp).arg(m_serverPort);
    qInfo() << "Server url: " + mBindingIpPort;

    connect(mWebSocketServer,
            &QWebSocketServer::serverError,
            mWebSocketServer,
            [=](QWebSocketProtocol::CloseCode closeCode) {
                Q_UNUSED(closeCode)
                emit errorOccurred(mWebSocketServer->errorString());
            });

    connect(mWebSocketServer, &QWebSocketServer::acceptError, mWebSocketServer, [=]() {
        emit errorOccurred(mWebSocketServer->errorString());
    });

    connect(mWebSocketServer, &QWebSocketServer::newConnection, mWebSocketServer, [=]() {
        QWebSocket *client = mWebSocketServer->nextPendingConnection();
        mWebSocketList.append(client);

        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        m_clients.append(ipPort);
        emit clientsChanged();

        connect(client, &QWebSocket::textMessageReceived, client, [=](const QString &message) {
            QByteArray bytes = message.toUtf8();
            QString ip = client->peerAddress().toString();
            quint16 port = client->peerPort();
            QString ipport = QString("%1:%2").arg(ip, QString::number(port));

            emit outputBytes(bytes);
            emit bytesRead(bytes, ipport);
        });

        connect(client, &QWebSocket::binaryMessageReceived, client, [=](const QByteArray &message) {
            QString ip = client->peerAddress().toString();
            quint16 port = client->peerPort();
            QString ipport = QString("%1:%2").arg(ip, QString::number(port));

            emit outputBytes(message);
            emit bytesRead(message, ipport);
        });

        connect(client, &QWebSocket::disconnected, client, [=]() {
            this->mWebSocketList.removeOne(client);
            this->m_clients.removeOne(ipPort);

            qInfo() << qPrintable(QString("Connection(%1) has been disconnected: %2")
                                      .arg(mBindingIpPort, client->errorString()));
            emit clientsChanged();
        });
        connect(client,
                X_TOOLS_WEB_SOCKET_ERROR_OCCURRED,
                client,
                [=](QAbstractSocket::SocketError err) {
                    Q_UNUSED(err);
                    this->mWebSocketList.removeOne(client);
                    this->m_clients.removeOne(ipPort);
                    qInfo() << QString("Error occurred: %1").arg(client->errorString());
                    emit clientsChanged();
                });
    });

    return true;
}

void xToolsWebSocketServerTool::writeBytes(const QByteArray &bytes)
{
    if (m_clientIndex >= 0 && m_clientIndex < mWebSocketList.length()) {
        QWebSocket *client = mWebSocketList.at(m_clientIndex);
        writeBytesInner(client, bytes);
    } else {
        for (auto &client : mWebSocketList) {
            writeBytesInner(client, bytes);
        }
    }
}

void xToolsWebSocketServerTool::uninitialize()
{
    mWebSocketServer->close();
    mWebSocketServer->deleteLater();
    mWebSocketServer = nullptr;
}

void xToolsWebSocketServerTool::writeBytesInner(QWebSocket *client, const QByteArray &bytes)
{
    qint64 ret = -1;
    QString hex;
    if (m_messageType == 0) {
        hex = QString::fromLatin1(xToolsByteArrayToHex(bytes, ' '));
        ret = client->sendBinaryMessage(bytes);
    } else {
        hex = QString::fromUtf8(bytes);
        ret = client->sendTextMessage(QString::fromUtf8(bytes));
    }

    if (ret == -1) {
        qWarning() << client->errorString();
    } else {
        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipport = QString("%1:%2").arg(ip, QString::number(port));
        qInfo() << QString("%1->%2:%3").arg(this->m_bindingIpPort, ipport, hex);
        emit bytesWritten(bytes, ipport);
    }
}
