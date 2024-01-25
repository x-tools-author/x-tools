/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakudpservertool.h"

#include "sakcompatibility.h"
#include "sakinterface.h"

SAKUdpServerTool::SAKUdpServerTool(QObject *parent)
    : SAKSocketServerTool{parent}
{}

bool SAKUdpServerTool::initialize(QString &errStr)
{
    m_udpSocket = new QUdpSocket();
    bool ret = false;
    if (mSpecifyIpAndPort) {
        ret = m_udpSocket->bind(QHostAddress(mServerIp), mServerPort);
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
    mBindingIpPort = QString("%1:%2").arg(ip).arg(port);
    qInfo() << mBindingIpPort;
    emit bindingIpPortChanged();

    connect(m_udpSocket, &QUdpSocket::readyRead, m_udpSocket, [=]() { readBytes(); });
    connect(m_udpSocket, SAK_SIG_SOCKETERROROCCURRED, this, [=]() {
        emit errorOccurred(m_udpSocket->errorString());
    });

    return true;
}

void SAKUdpServerTool::writeBytes(const QByteArray &bytes)
{
    if (mClientIndex >= 0 && mClientIndex < mClients.length()) {
        QString ipPort = mClients.at(mClientIndex);
        QStringList list = ipPort.split(":");
        QString ip = list.first();
        quint16 port = list.last().toInt();
        writeDatagram(bytes, ip, port);
    } else {
        for (auto &client : mClients) {
            QStringList list = client.split(":");
            QString ip = list.first();
            quint16 port = list.last().toInt();
            writeDatagram(bytes, ip, port);
        }
    }
}

void SAKUdpServerTool::uninitialize()
{
    m_udpSocket->deleteLater();
    m_udpSocket = nullptr;
}

void SAKUdpServerTool::readBytes()
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

        QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
        QString hex = QString::fromLatin1(ba);
        QString info = QString("%1:%2").arg(address.toString()).arg(port);
        QString msg = QString("%1<-%2:%3").arg(mBindingIpPort, info, hex);
        qInfo() << msg;

        if (!mClients.contains(info)) {
            mClients.append(info);
            emit clientsChanged();
        }

        emit outputBytes(bytes);
        emit bytesRead(bytes, info);
    }
}

void SAKUdpServerTool::writeDatagram(const QByteArray &bytes,
                                     const QString &ip,
                                     quint16 port)
{
    qint64 ret = m_udpSocket->writeDatagram(bytes, QHostAddress(ip), port);
    if (ret == -1) {
        qWarning() << m_udpSocket->errorString();
    } else {
        QString hex = QString::fromLatin1(SAKInterface::arrayToHex(bytes, ' '));
        qInfo() << QString("%1<-%2").arg(mBindingIpPort, hex);
        emit bytesWritten(bytes, mBindingIpPort);
    }
}
