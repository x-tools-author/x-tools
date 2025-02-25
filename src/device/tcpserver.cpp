/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tcpserver.h"

#include <QTcpSocket>

namespace xTools {

TcpServer::TcpServer(QObject *parent)
    : SocketServer(parent)
{}

TcpServer::~TcpServer() {}

QObject *TcpServer::initDevice()
{
    m_tcpServer = new QTcpServer();
    connect(m_tcpServer, &QTcpServer::newConnection, m_tcpServer, [this]() {
        QTcpSocket *client = m_tcpServer->nextPendingConnection();
        qInfo() << "New connection from" << client->peerAddress().toString() << client->peerPort();
        setupClient(client);
    });
    connect(m_tcpServer, &QTcpServer::acceptError, m_tcpServer, [this]() {
        emit errorOccurred(m_tcpServer->errorString());
    });

    if (!m_tcpServer->listen(QHostAddress(m_serverAddress), m_serverPort)) {
        m_tcpServer->deleteLater();
        m_tcpServer = nullptr;
        return nullptr;
    }

    qInfo() << "The server is listening on" << m_serverAddress << m_serverPort;
    return m_tcpServer;
}

void TcpServer::deinitDevice()
{
    disconnectAllClients();

    m_tcpServer->close();
    m_tcpServer->deleteLater();
    m_tcpServer = nullptr;
}

void TcpServer::writeBytes(const QByteArray &bytes)
{
    QString flag = currentClientFlag();
    if (flag.isEmpty()) {
        for (auto client : m_sockets) {
            writeBytes(client, bytes);
        }
    } else {
        for (auto client : m_sockets) {
            if (makeFlag(client->peerAddress().toString(), client->peerPort()) == flag) {
                writeBytes(client, bytes);
                break;
            }
        }
    }
}

void TcpServer::disconnectAllClients()
{
    for (auto client : m_sockets) {
        client->disconnectFromHost();
        client->close();
        client->deleteLater();
    }
    m_sockets.clear();
}

void TcpServer::writeBytes(QTcpSocket *socket, const QByteArray &bytes)
{
    qint64 ret = socket->write(bytes);
    if (ret == bytes.length()) {
        emit bytesWritten(bytes, makeFlag(socket->peerAddress().toString(), socket->peerPort()));
    } else {
        emit errorOccurred(socket->errorString());
    }
}

void TcpServer::setupClient(QTcpSocket *socket)
{
    m_sockets.append(socket);
    addClient(makeFlag(socket->peerAddress().toString(), socket->peerPort()));

    connect(socket, &QTcpSocket::readyRead, socket, [socket, this]() { readBytes(socket); });
    connect(socket, &QTcpSocket::disconnected, socket, [socket, this]() { removeSocket(socket); });
    connect(socket, &QTcpSocket::errorOccurred, socket, [socket, this]() { removeSocket(socket); });
}

void TcpServer::readBytes(QTcpSocket *socket)
{
    QString currentFlag = currentClientFlag();
    if (currentFlag.isEmpty()) {
        QByteArray bytes = socket->readAll();
        if (!bytes.isEmpty()) {
            emit bytesRead(bytes, makeFlag(socket->peerAddress().toString(), socket->peerPort()));
        }
    } else if (currentFlag == makeFlag(socket->peerAddress().toString(), socket->peerPort())) {
        QByteArray bytes = socket->readAll();
        if (!bytes.isEmpty()) {
            emit bytesRead(bytes, currentFlag);
        }
    } else {
        socket->readAll();
    }
}

void TcpServer::removeSocket(QTcpSocket *socket)
{
    removeClient(makeFlag(socket->peerAddress().toString(), socket->peerPort()));
    socket->deleteLater();
    m_sockets.removeAll(socket);
}

} // namespace xTools
