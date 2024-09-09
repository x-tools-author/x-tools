/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WebSocketServer.h"

#include <QWebSocket>

#include "../../xIO.h"

namespace xTools {

WebSocketServer::WebSocketServer(QObject *parent)
    : SocketServer(parent)
{}

WebSocketServer::~WebSocketServer() {}

QObject *WebSocketServer::initDevice()
{
    m_webSocketServer = new QWebSocketServer(QStringLiteral("WebSocket Server"),
                                             QWebSocketServer::NonSecureMode);
    connect(m_webSocketServer, &QWebSocketServer::acceptError, m_webSocketServer, [this]() {
        emit errorOccurred(m_webSocketServer->errorString());
    });
    connect(m_webSocketServer, &QWebSocketServer::newConnection, m_webSocketServer, [this]() {
        QWebSocket *socket = this->m_webSocketServer->nextPendingConnection();
        qInfo() << "New connection:" << socket->peerAddress().toString() << socket->peerPort();

        m_sockets.append(socket);
        this->setupSocket(socket);
    });

    if (!m_webSocketServer->listen(QHostAddress(m_serverAddress), m_serverPort)) {
        m_webSocketServer->deleteLater();
        m_webSocketServer = nullptr;

        qWarning() << "WebSocketServer: listen failed";

        return nullptr;
    }

    qInfo("Web socket server info:%s:%d", m_serverAddress.toLatin1().data(), m_serverPort);

    return m_webSocketServer;
}

void WebSocketServer::deinitDevice()
{
    if (m_webSocketServer) {
        m_webSocketServer->close();
        m_webSocketServer->deleteLater();
        m_webSocketServer = nullptr;
    }
}

void WebSocketServer::writeBytes(const QByteArray &bytes)
{
    QString currentFlag = currentClientFlag();
    if (currentFlag.isEmpty()) {
        for (auto &socket : m_sockets) {
            writeBytes(socket, bytes);
        }
    } else {
        for (auto &socket : m_sockets) {
            if (currentFlag == makeFlag(socket->peerAddress().toString(), socket->peerPort())) {
                writeBytes(socket, bytes);
                break;
            }
        }
    }
}

void WebSocketServer::setupSocket(QWebSocket *socket)
{
    QString flag = makeFlag(socket->peerAddress().toString(), socket->peerPort());
    addClient(flag);

    connect(socket, &QWebSocket::disconnected, socket, [this, flag]() { removeClient(flag); });
    connect(socket, &QWebSocket::textMessageReceived, socket, [=](const QString &message) {
        onTextMessageReceived(socket, message);
    });
    connect(socket, &QWebSocket::binaryMessageReceived, socket, [=](const QByteArray &message) {
        onBinaryMessageReceived(socket, message);
    });
    //TODO:qt6
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    connect(socket, &QWebSocket::errorOccurred, socket, [this, flag, socket]() {
        this->m_sockets.removeAll(socket);
        this->removeClient(flag);
    });
#else
    connect(socket,
            QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            socket,
            [this, flag, socket]() {
                this->m_sockets.removeAll(socket);
                this->removeClient(flag);
            });
#endif
    connect(socket, &QWebSocket::disconnected, socket, [this, flag, socket]() {
        this->m_sockets.removeAll(socket);
        this->removeClient(flag);
    });
}

void WebSocketServer::writeBytes(QWebSocket *socket, const QByteArray &bytes)
{
    const QString flag = makeFlag(socket->peerAddress().toString(), socket->peerPort()) + "[B]";
    if (m_channel == static_cast<int>(xIO::WebSocketDataChannel::Binary)) {
        if (socket->sendBinaryMessage(bytes) == bytes.size()) {
            emit bytesWritten(bytes, flag);
        } else {
            qInfo() << "WebSocketServer: sendBinaryMessage failed:" << socket->errorString();
        }
    } else if (m_channel == static_cast<int>(xIO::WebSocketDataChannel::Text)) {
        if (socket->sendTextMessage(QString::fromUtf8(bytes)) == bytes.size()) {
            emit bytesWritten(bytes, flag);
        } else {
            qInfo() << "WebSocketServer: sendTextMessage failed:" << socket->errorString();
        }
    }
}

void WebSocketServer::onTextMessageReceived(QWebSocket *socket, const QString &message)
{
    const QString socketFlag = makeFlag(socket->peerAddress().toString(), socket->peerPort());
    const QString currentFlag = currentClientFlag();
    if (currentFlag.isEmpty()) {
        emit bytesRead(message.toUtf8(), socketFlag + "[T]");
    } else {
        if (currentFlag == socketFlag) {
            emit bytesRead(message.toUtf8(), socketFlag + "[T]");
        }
    }
}

void WebSocketServer::onBinaryMessageReceived(QWebSocket *socket, const QByteArray &message)
{
    QString currentFlag = currentClientFlag();
    if (currentFlag.isEmpty()) {
        QString flag = makeFlag(m_serverAddress, m_serverPort);
        emit bytesRead(message, flag + "[T]");
    } else {
        if (currentFlag == makeFlag(socket->peerAddress().toString(), socket->peerPort())) {
            emit bytesRead(message, currentFlag + "[T]");
        }
    }
}

} // namespace xTools
