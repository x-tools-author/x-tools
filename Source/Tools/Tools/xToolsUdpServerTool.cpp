/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsUdpServerTool.h"

#include "xToolsCompatibility.h"

xToolsUdpServerTool::xToolsUdpServerTool(QObject *parent)
    : xToolsSocketServerTool{parent}
{}

bool xToolsUdpServerTool::initialize(QString &errStr)
{
    m_udpSocket = new QUdpSocket();
    bool ret = false;
    if (m_specifyIpAndPort) {
        ret = m_udpSocket->bind(QHostAddress(m_serverIp), m_serverPort);
    } else {
        ret = m_udpSocket->bind();
    }

    if (!ret) {
        qWarning() << m_udpSocket->errorString();
        errStr = m_udpSocket->errorString();
        return false;
    }

    QString ip = m_udpSocket->localAddress().toString();
    int port = m_udpSocket->localPort();
    m_bindingIpPort = QString("%1:%2").arg(ip).arg(port);
    qInfo() << m_bindingIpPort;
    emit bindingIpPortChanged();

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [=]() { readBytes(); });
    connect(m_udpSocket, X_TOOLS_ABSTRACT_SOCKET_ERROR_OCCURRED, this, [=]() {
        emit errorOccurred(m_udpSocket->errorString());
    });

    return true;
}

void xToolsUdpServerTool::writeBytes(const QByteArray &bytes)
{
    if (m_clientIndex >= 0 && m_clientIndex < m_clients.length()) {
        QString ipPort = m_clients.at(m_clientIndex);
        QStringList list = ipPort.split(":");
        QString ip = list.first();
        quint16 port = list.last().toInt();
        writeDatagram(bytes, ip, port);
    } else {
        for (auto &client : m_clients) {
            QStringList list = client.split(":");
            QString ip = list.first();
            quint16 port = list.last().toInt();
            writeDatagram(bytes, ip, port);
        }
    }
}

void xToolsUdpServerTool::uninitialize()
{
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void xToolsUdpServerTool::readBytes()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        auto len = m_udpSocket->pendingDatagramSize();
        if (len == -1) {
            break;
        }

        QByteArray bytes(len, 0);
        QHostAddress address;
        quint16 port;
        qint64 ret = m_udpSocket->readDatagram(bytes.data(), bytes.length(), &address, &port);
        if (ret == -1) {
            qWarning() << m_udpSocket->errorString();
            break;
        }

        QByteArray ba = xToolsByteArrayToHex(bytes, ' ');
        QString hex = QString::fromLatin1(ba);
        QString info = QString("%1:%2").arg(address.toString()).arg(port);
        QString msg = QString("%1<-%2:%3").arg(m_bindingIpPort, info, hex);
        qInfo() << msg;

        if (!m_clients.contains(info)) {
            m_clients.append(info);
            emit clientsChanged();
        }

        emit outputBytes(bytes);
        emit bytesRead(bytes, info);
    }
}

void xToolsUdpServerTool::writeDatagram(const QByteArray &bytes,
                                     const QString &ip,
                                     quint16 port)
{
    qint64 ret = m_udpSocket->writeDatagram(bytes, QHostAddress(ip), port);
    if (ret == -1) {
        qWarning() << m_udpSocket->errorString();
    } else {
        QString hex = QString::fromLatin1(xToolsByteArrayToHex(bytes, ' '));
        qInfo() << QString("%1<-%2").arg(m_bindingIpPort, hex);
        emit bytesWritten(bytes, m_bindingIpPort);
    }
}
