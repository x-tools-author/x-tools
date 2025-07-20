/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpserver.h"

#include "common/xtools.h"

UdpServer::UdpServer(QObject *parent)
    : SocketServer(parent)
{}

UdpServer::~UdpServer() {}

QObject *UdpServer::initDevice()
{
    m_udpSocket = new QUdpSocket();
    if (!m_udpSocket->bind(QHostAddress(m_serverAddress), m_serverPort)) {
        qWarning() << "Failed to bind to address" << m_serverAddress << "and port" << m_serverPort;
        m_udpSocket->deleteLater();
        m_udpSocket = nullptr;
        return nullptr;
    }

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [this]() { readPendingDatagrams(); });
    connect(m_udpSocket, xUdpSocketErrorOccurred, m_udpSocket, [this]() {
        if (m_udpSocket->error() == QAbstractSocket::ConnectionRefusedError) {
            return;
        }

        emit errorOccurred(m_udpSocket->errorString());
    });

    qInfo() << "Udp server is listening on" << m_serverAddress << "and port" << m_serverPort;
    return m_udpSocket;
}

void UdpServer::deinitDevice()
{
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void UdpServer::writeActually(const QByteArray &bytes)
{
    QString currentFlag = currentClientFlag();
    if (currentFlag.isEmpty()) {
        QStringList flags = clients();
        for (auto &flag : flags) {
            writeDatagram(bytes, flag);
        }
    } else {
        QPair<QString, int> client = splitFlag(currentFlag);
        if (isValidFlag(client)) {
            writeDatagram(bytes, currentFlag);
        }
    }
}

void UdpServer::disconnectAllClients()
{
    clearClients();
}

void UdpServer::readPendingDatagrams()
{
    QString currentFlag = currentClientFlag();
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        if (m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort) < 1) {
            continue;
        }

        QString const flag = makeFlag(sender.toString(), senderPort);
        addClient(flag);
        if (currentFlag.isEmpty()) {
            emit bytesRead(datagram, flag);
        } else if (currentFlag == flag) {
            emit bytesRead(datagram, flag);
        }
    }
}

void UdpServer::writeDatagram(const QByteArray &bytes, const QString &flag)
{
    QPair<QString, int> client = splitFlag(flag);
    if (!isValidFlag(client)) {
        return;
    }

    QString const address = client.first;
    quint16 const port = client.second;
    qint64 ret = m_udpSocket->writeDatagram(bytes, QHostAddress(address), port);
    if (ret == bytes.length()) {
        if (m_udpSocket->error() == QAbstractSocket::ConnectionRefusedError) {
            removeClient(flag);
        } else {
            emit bytesWritten(bytes, makeFlag(address, port));
        }
    } else {
        removeClient(flag);
    }
}
