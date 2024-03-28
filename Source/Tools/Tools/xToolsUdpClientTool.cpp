/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsUdpClientTool.h"

#include <QHostAddress>

#include "xToolsCompatibility.h"

xToolsUdpClientTool::xToolsUdpClientTool(QObject *parent)
    : xToolsSocketClientTool{parent}
{}

bool xToolsUdpClientTool::initialize(QString &errStr)
{
    m_udpSocket = new QUdpSocket();
    if (m_specifyClientIpPort) {
        if (!m_udpSocket->bind(QHostAddress(m_clientIp), m_clientPort)) {
            errStr = m_udpSocket->errorString();
            return false;
        }
    } else {
        if (!m_udpSocket->bind()) {
            errStr = m_udpSocket->errorString();
            return false;
        }
    }

    QString ip = m_udpSocket->localAddress().toString();
    int port = m_udpSocket->localPort();
    QString info = QString("%1:%2").arg(ip).arg(port);
    m_bindingIpPort = info;
    qInfo() << qPrintable(QString("bind to %1").arg(info));
    emit bindingIpPortChanged();

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [=]() { readBytes(); });
    connect(m_udpSocket, X_TOOLS_ABSTRACT_SOCKET_ERROR_OCCURRED, this, [=]() {
        emit errorOccurred(m_udpSocket->errorString());
    });

    return true;
}

void xToolsUdpClientTool::writeBytes(const QByteArray &bytes)
{
    if (m_serverIp.isEmpty()) {
        return;
    }

    qint64 ret = m_udpSocket->writeDatagram(bytes, QHostAddress(m_serverIp), m_serverPort);
    QString ipport = m_serverIp + ":" + QString::number(m_serverPort);
    if (ret == -1) {
        QString str = m_udpSocket->errorString();
        qInfo() << qPrintable(QString("write bytes to %1 error: %2").arg(ipport, str));
    } else {
#ifdef QT_DEBUG
        QByteArray ba = xToolsByteArrayToHex(bytes, ' ');
        QString hex = QString::fromLatin1(ba);
        QString portStr = QString::number(m_serverPort);
        QString serverInfo = QString("%1:%2").arg(m_serverIp, portStr);
        QString info = m_bindingIpPort + "->" + serverInfo + ":" + hex;
        qInfo() << qPrintable(info);
#endif
        emit bytesWritten(bytes, ipport);
    }
}

void xToolsUdpClientTool::uninitialize()
{
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void xToolsUdpClientTool::readBytes()
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
        } else {
            QByteArray ba = xToolsByteArrayToHex(bytes, ' ');
            QString hex = QString::fromLatin1(ba);
            QString portStr = address.toString();
            QString serverInfo = address.toString() + ":" + portStr;
            QString info = m_bindingIpPort + "<-" + serverInfo + ":" + hex;
            qInfo() << qPrintable(info);
            emit outputBytes(bytes);
            emit bytesRead(bytes, m_bindingIpPort);
        }
    }
}
