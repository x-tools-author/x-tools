/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTcpServerTool.h"

#include <QTcpSocket>

#include "xToolsCompatibility.h"

xToolsTcpServerTool::xToolsTcpServerTool(QObject *parent)
    : xToolsSocketServerTool{parent}
{}

bool xToolsTcpServerTool::initialize(QString &errStr)
{
    m_tcpServer = new QTcpServer();
    if (!m_tcpServer->listen(QHostAddress(m_serverIp), m_serverPort)) {
        errStr = "Error occurred: " + m_tcpServer->errorString();
        return false;
    }

    m_bindingIpPort = QString("%1:%2").arg(m_serverIp).arg(m_serverPort);
    qInfo() << "Server ip and port: " + m_bindingIpPort;

    connect(m_tcpServer, &QTcpServer::newConnection, m_tcpServer, [=]() {
        QTcpSocket *client = m_tcpServer->nextPendingConnection();
        this->m_tcpSocketListMutex.lock();
        this->m_tcpSocketList.append(client);
        this->m_tcpSocketListMutex.unlock();

        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        m_clients.append(ipPort);
        qInfo() << "New connection:" + ipPort;
        emit clientsChanged();

        connect(client, &QTcpSocket::readyRead, client, [=]() {
            QByteArray bytes = client->readAll();
            QString hex = bytes.toHex();
            QString msg = QString("%1<-%2:%3").arg(m_bindingIpPort, ipPort, hex);
            qInfo() << msg;
            emit outputBytes(bytes);
            emit bytesRead(bytes, ipPort);
        });

        connect(client, &QTcpSocket::disconnected, client, [=]() {
            client->deleteLater();
            this->m_tcpSocketListMutex.lock();
            this->m_tcpSocketList.removeOne(client);
            this->m_tcpSocketListMutex.unlock();
            this->m_clients.removeOne(ipPort);
            qInfo() << QString("Connection(%1) has been disconnected: %2")
                           .arg(ipPort, client->errorString());
            emit clientsChanged();
        });

        connect(client, X_TOOLS_ABSTRACT_SOCKET_ERROR_OCCURRED, client, [=]() {
            this->m_tcpSocketListMutex.lock();
            this->m_tcpSocketList.removeOne(client);
            this->m_tcpSocketListMutex.unlock();
            this->m_clients.removeOne(ipPort);
            qInfo() << QString("Error occurred: %1").arg(client->errorString());
            emit clientsChanged();
        });
    });

    return true;
}

void xToolsTcpServerTool::writeBytes(const QByteArray &bytes)
{
    if (m_clientIndex >= 0 && m_clientIndex < m_tcpSocketList.length()) {
        QTcpSocket *client = m_tcpSocketList.at(m_clientIndex);
        writeBytesInner(client, bytes);
    } else {
        for (auto &client : m_tcpSocketList) {
            writeBytesInner(client, bytes);
        }
    }
}

void xToolsTcpServerTool::uninitialize()
{
    m_tcpServer->close();
    m_tcpServer->deleteLater();
    m_tcpServer = nullptr;
}

void xToolsTcpServerTool::writeBytesInner(QTcpSocket *client, const QByteArray &bytes)
{
    qint64 ret = client->write(bytes);
    if (ret == -1) {
        qWarning() << m_tcpServer->errorString();
    } else {
        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        QString hex = bytes.toHex();
        qInfo() << QString("%1->%2:%3").arg(m_bindingIpPort, ipPort, hex);
        emit bytesWritten(bytes, ipPort);
    }
}
