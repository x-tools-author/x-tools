/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent)
    : SocketClient(parent)
{}

UdpClient::~UdpClient() {}

QObject *UdpClient::initDevice()
{
    m_udpSocket = new QUdpSocket();
    if (!m_udpSocket->bind(QHostAddress(m_clientAddress), m_clientPort)) {
        qWarning() << "Failed to bind to address" << m_clientAddress << "and port" << m_clientPort;
        m_udpSocket->deleteLater();
        m_udpSocket = nullptr;
        return nullptr;
    }

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [this]() { readPendingDatagrams(); });
    connect(m_udpSocket, &QUdpSocket::errorOccurred, m_udpSocket, [this]() {
        qWarning() << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    });

    qInfo() << "client address:" << m_clientAddress << "port:" << m_clientPort
            << "server address:" << m_serverAddress << "port:" << m_serverPort;

    return m_udpSocket;
}

void UdpClient::deinitDevice()
{
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void UdpClient::writeBytesToDevice(const QByteArray &bytes)
{
    qint64 ret = m_udpSocket->writeDatagram(bytes, QHostAddress(m_serverAddress), m_serverPort);
    if (ret == bytes.length()) {
        emit bytesWritten(bytes, makeFlag(m_serverAddress, m_serverPort));
    } else {
        emit errorOccurred(m_udpSocket->errorString());
    }
}

void UdpClient::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        if (m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort) > 0) {
            emit bytesRead(datagram, makeFlag(sender.toString(), senderPort));
        }
    }
}
