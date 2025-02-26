/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tcpclient.h"

#include <QHostAddress>

TcpClient::TcpClient(QObject *parent)
    : SocketClient(parent)
{}

TcpClient::~TcpClient() {}

QObject *TcpClient::initDevice()
{
    m_tcpSocket = new QTcpSocket();
    connect(m_tcpSocket, &QTcpSocket::readyRead, m_tcpSocket, [this]() { readBytesFromDevice(); });
    connect(m_tcpSocket, &QTcpSocket::disconnected, m_tcpSocket, [this]() {
#if 0
        emit errorOccurred("Disconnected!");
#endif
    });
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, m_tcpSocket, [this]() {
        emit errorOccurred(m_tcpSocket->errorString());
    });
#if 0
    if (!m_tcpSocket->bind(QHostAddress(m_clientAddress), m_clientPort)) {
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;

        qInfo() << "Failed to bind to address" << m_clientAddress << "and port"
                << QString::number(m_clientPort);

        return nullptr;
    }
#endif
    m_tcpSocket->connectToHost(m_serverAddress, m_serverPort);
    if (!m_tcpSocket->waitForConnected()) {
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
        return nullptr;
    }

    qInfo() << "server address:" << m_serverAddress << "port:" << m_serverPort;
    return m_tcpSocket;
}

void TcpClient::deinitDevice()
{
    m_tcpSocket->disconnectFromHost();
    m_tcpSocket->close();
    m_tcpSocket->deleteLater();
    m_tcpSocket = nullptr;
}

void TcpClient::writeBytes(const QByteArray &bytes)
{
    qint64 ret = m_tcpSocket->write(bytes);
    if (ret == bytes.length()) {
        emit bytesWritten(bytes, makeFlag(m_serverAddress, m_serverPort));
    } else {
        emit errorOccurred(m_tcpSocket->errorString());
    }
}

void TcpClient::readBytesFromDevice()
{
    QByteArray bytes = m_tcpSocket->readAll();
    emit bytesRead(bytes, makeFlag(m_serverAddress, m_serverPort));
}
