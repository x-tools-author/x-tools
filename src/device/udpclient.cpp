/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpclient.h"

namespace xTools {

UdpClient::UdpClient(QObject *parent)
    : SocketClient(parent)
{}

UdpClient::~UdpClient() {}

QObject *UdpClient::initDevice()
{
    m_udpSocket = new QUdpSocket();
    if (!m_udpSocket->open(QUdpSocket::ReadWrite)) {
        qWarning() << "Failed to open udp socket:" << m_udpSocket->errorString();
        m_udpSocket->deleteLater();
        m_udpSocket = nullptr;
    }
#if 1
    if (!m_udpSocket->bind(QHostAddress(m_clientAddress), m_clientPort)) {
        qWarning() << "Failed to bind to address" << m_clientAddress << "and port" << m_clientPort
                   << ":" << m_udpSocket->errorString();
        m_udpSocket->deleteLater();
        m_udpSocket = nullptr;
        return nullptr;
    } else {
        qInfo() << "UDP client address:" << m_clientAddress << "port:" << m_clientPort;
    }
#endif

    if (m_enableMulticast) {
        if (!m_udpSocket->joinMulticastGroup(QHostAddress(m_multicastAddress))) {
            qWarning() << "Failed to join multicast group:" << m_multicastAddress << ":"
                       << m_udpSocket->errorString();
            m_udpSocket->deleteLater();
            m_udpSocket = nullptr;
            return nullptr;
        } else {
            qInfo() << "Joined multicast group:" << m_multicastAddress
                    << ", port is:" << m_multicastPort;
        }
    }

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [this]() { readPendingDatagrams(); });
    connect(m_udpSocket, &QUdpSocket::errorOccurred, m_udpSocket, [this]() {
        qWarning() << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    });

    qInfo() << "UDP server address:" << m_serverAddress << "port:" << m_serverPort;

    return m_udpSocket;
}

void UdpClient::deinitDevice()
{
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void UdpClient::writeBytes(const QByteArray &bytes)
{
    if (m_enableMulticast) {
        writeDatagram(bytes, m_multicastAddress, m_multicastPort);
    }

    if (!(m_enableMulticast && m_justMulticast)) {
        writeDatagram(bytes, m_serverAddress, m_serverPort);
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

void UdpClient::writeDatagram(const QByteArray &bytes, const QString &ip, quint16 port)
{
    qint64 ret = m_udpSocket->writeDatagram(bytes, QHostAddress(ip), port);
    if (ret == bytes.length()) {
        emit bytesWritten(bytes, makeFlag(ip, port));
    } else {
        qWarning() << "Failed to write bytes:" << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    }
}

} // namespace xTools
