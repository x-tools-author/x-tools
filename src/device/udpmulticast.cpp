/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpmulticast.h"

#include <QDebug>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>

#include "common/xtools.h"
#include "utilities/compatibility.h"

UdpMulticastParameters variantMap2UdpMulticastParameters(const QVariantMap &obj)
{
    const QString defaultAddress = "239.168.3.255";
    const int defaultPort = 54132;
    UdpMulticastParameters params;
    params.address = obj.value("address", defaultAddress).toString();
    params.port = static_cast<quint16>(obj.value("port", defaultPort).toInt());
    params.name = obj.value("name").toString();
    params.loopback = obj.value("loopback", false).toBool();

    if (params.address.isEmpty()) {
        params.address = defaultAddress;
    }

    if (params.port <= 0 || params.port > 65535) {
        params.port = defaultPort;
    }

    return params;
}

QVariantMap udpMulticastParameters2variantMap(const UdpMulticastParameters &params)
{
    const QString defaultAddress = "239.168.3.255";
    const int defaultPort = 54132;

    QVariantMap obj;
    obj.insert("address", params.address.isEmpty() ? defaultAddress : params.address);
    obj.insert("port", params.port <= 0 || params.port > 65535 ? defaultPort : params.port);
    obj.insert("name", params.name);
    obj.insert("loopback", params.loopback);
    return obj;
}

UdpMulticast::UdpMulticast(QObject *parent)
    : Device(parent)
{}

UdpMulticast::~UdpMulticast()
{
    if (isRunning()) {
        closeDevice();
    }
}

QObject *UdpMulticast::initDevice()
{
    QVariantMap tmp = save();
    m_parameters = variantMap2UdpMulticastParameters(tmp);
    QNetworkInterface interface = QNetworkInterface::interfaceFromName(m_parameters.name);
    if (!interface.isValid()) {
        QString errorMsg = tr("Invalid network interface: %1").arg(m_parameters.name);
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        return Q_NULLPTR;
    }

    if (!interface.flags().testFlag(QNetworkInterface::CanMulticast)) {
        QString errorMsg = tr("Network interface %1 does not support multicast")
                               .arg(m_parameters.name);
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        return Q_NULLPTR;
    }

    QHostAddress hostAddress;
    QList<QNetworkAddressEntry> entries = interface.addressEntries();
    for (const QNetworkAddressEntry &entry : const_cast<QList<QNetworkAddressEntry> &>(entries)) {
        if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            hostAddress = QHostAddress(entry.ip());
            break;
        }
    }

    m_udpSocket = new QUdpSocket();
    if (!m_udpSocket->bind(hostAddress, m_parameters.port)) {
        QString errorMsg = tr("Failed to bind UDP socket: %1").arg(m_udpSocket->errorString());
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        m_udpSocket->deleteLater();
        m_udpSocket = Q_NULLPTR;
        return Q_NULLPTR;
    }

    if (!m_udpSocket->joinMulticastGroup(QHostAddress(m_parameters.address), interface)) {
        QString errorMsg = tr("Failed to join multicast group %1:%2: %3")
                               .arg(m_parameters.address)
                               .arg(m_parameters.port)
                               .arg(m_udpSocket->errorString());
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        m_udpSocket->deleteLater();
        m_udpSocket = Q_NULLPTR;
        return Q_NULLPTR;
    }

    m_udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, m_parameters.loopback);
    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [this]() { readPendingDatagrams(); });
    connect(m_udpSocket, xUdpSocketErrorOccurred, m_udpSocket, [this]() {
        qWarning() << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    });

    return m_udpSocket;
}

void UdpMulticast::deinitDevice()
{
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void UdpMulticast::writeActually(const QByteArray &bytes)
{
    qint64 ret = m_udpSocket->writeDatagram(bytes,
                                            QHostAddress(m_parameters.address),
                                            m_parameters.port);
    if (ret == bytes.length()) {
        QString flag = QString("%1:%2").arg(m_parameters.address).arg(m_parameters.port);
        emit bytesWritten(bytes, flag);
    } else {
        qWarning() << "Failed to write bytes:" << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    }
}

void UdpMulticast::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QHostAddress sender;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        if (m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort) > 0) {
            QString flag = QString("%1:%2").arg(sender.toString()).arg(senderPort);
            emit bytesRead(datagram, flag);
        }
    }
}
