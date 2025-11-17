/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpbroadcast.h"

#include <QDebug>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>

#include "common/xtools.h"
#include "utilities/compatibility.h"

UdpBroadcastParameters variantMap2UdpBroadcastParameters(const QVariantMap &obj)
{
    const QString defaultAddress = "255.255.255.255";
    const int defaultPort = 54132;
    UdpBroadcastParameters params;
    params.address = obj.value("address", defaultAddress).toString();
    params.port = static_cast<quint16>(obj.value("port", defaultPort).toInt());

    if (params.address.isEmpty()) {
        params.address = defaultAddress;
    }

    if (params.port <= 0 || params.port > 65535) {
        params.port = defaultPort;
    }

    return params;
}

QVariantMap udpBroadcastParameters2variantMap(const UdpBroadcastParameters &params)
{
    const QString defaultAddress = "255.255.255.255";
    const int defaultPort = 54132;

    QVariantMap obj;
    obj.insert("address", params.address.isEmpty() ? defaultAddress : params.address);
    obj.insert("port", params.port <= 0 || params.port > 65535 ? defaultPort : params.port);
    return obj;
}

UdpBroadcast::UdpBroadcast(QObject *parent)
    : Device(parent)
{}

UdpBroadcast::~UdpBroadcast()
{
    if (isRunning()) {
        closeDevice();
    }
}

QObject *UdpBroadcast::initDevice()
{
    QVariantMap tmp = save();
    m_parameters = variantMap2UdpBroadcastParameters(tmp);
    m_udpSocket = new QUdpSocket();
    if (!m_udpSocket->bind(QHostAddress::AnyIPv4)) {
        QString errorMsg = tr("Failed to bind UDP socket: %1").arg(m_udpSocket->errorString());
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        m_udpSocket->deleteLater();
        m_udpSocket = Q_NULLPTR;
        return Q_NULLPTR;
    }

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [this]() { readPendingDatagrams(); });
    connect(m_udpSocket, xUdpSocketErrorOccurred, m_udpSocket, [this]() {
        qWarning() << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    });

    return m_udpSocket;
}

void UdpBroadcast::deinitDevice()
{
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void UdpBroadcast::writeActually(const QByteArray &bytes)
{
    QString localAddress = m_udpSocket->localAddress().toString();
    quint16 localPort = m_udpSocket->localPort();
    QString from = QString("%1:%2").arg(localAddress).arg(localPort);
    QString to = QString("%1:%2").arg(m_parameters.address).arg(m_parameters.port);
    qint64 ret = m_udpSocket->writeDatagram(bytes,
                                            QHostAddress(m_parameters.address),
                                            m_parameters.port);
    if (ret == bytes.length()) {
        emit bytesWritten(bytes, QString("%1->%2").arg(from, to));
    } else {
        qWarning() << "Failed to write bytes:" << m_udpSocket->errorString();
        emit errorOccurred(m_udpSocket->errorString());
    }
}

void UdpBroadcast::readPendingDatagrams()
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
