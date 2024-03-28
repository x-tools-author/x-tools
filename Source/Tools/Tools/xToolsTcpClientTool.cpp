/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTcpClientTool.h"

#include <QHostAddress>

#include "xToolsCompatibility.h"

xToolsTcpClientTool::xToolsTcpClientTool(QObject* parent)
    : xToolsSocketClientTool{parent}
{}

bool xToolsTcpClientTool::initialize(QString& errStr)
{
    mTcpSocket = new QTcpSocket();
    if (m_specifyClientIpPort) {
        if (!mTcpSocket->bind(QHostAddress(m_clientIp), m_clientPort)) {
            errStr = "Binding error: " + mTcpSocket->errorString();
            return false;
        }
    }
    
    mTcpSocket->connectToHost(QHostAddress(m_serverIp), m_serverPort);
    if (!mTcpSocket->waitForConnected()) {
        errStr = "Connect to host error: " + mTcpSocket->errorString();
        return false;
    }

    QString ip = mTcpSocket->localAddress().toString();
    QString port = QString::number(mTcpSocket->localPort());
    m_bindingIpPort = ip + ":" + port;
    qInfo() << "Client address and port: " + m_bindingIpPort;
    emit bindingIpPortChanged();

    connect(mTcpSocket, X_TOOLS_ABSTRACT_SOCKET_ERROR_OCCURRED, mTcpSocket, [=]() {
        QString info = "Error occurred: " + mTcpSocket->errorString();
        qInfo() << info;
        emit errorOccurred(mTcpSocket->errorString());
    });

    connect(mTcpSocket, &QTcpSocket::disconnected, mTcpSocket, [=]() { exit(); });

    connect(mTcpSocket, &QTcpSocket::readyRead, mTcpSocket, [=]() { readBytes(); });

    return true;
}

void xToolsTcpClientTool::writeBytes(const QByteArray& bytes)
{
    qint64 ret = mTcpSocket->write(bytes);
    if (ret == -1) {
        qWarning() << mTcpSocket->errorString();
    } else {
        emit bytesWritten(bytes, m_bindingIpPort);
    }
}

void xToolsTcpClientTool::uninitialize()
{
    mTcpSocket->disconnectFromHost();
    mTcpSocket->close();
    mTcpSocket->deleteLater();
    mTcpSocket = nullptr;
}

void xToolsTcpClientTool::readBytes()
{
    QHostAddress address = mTcpSocket->peerAddress();
    quint16 port = mTcpSocket->peerPort();
    QByteArray bytes = mTcpSocket->readAll();
    if (!bytes.isEmpty()) {
        QByteArray ba = xToolsByteArrayToHex(bytes, ' ');
        QString ipport = address.toString() + ":" + QString::number(port);
        QString info = m_bindingIpPort + "<-" + ipport + ":";
        info += QString::fromLatin1(ba);
        qInfo() << info;
        emit outputBytes(bytes);
        emit bytesRead(bytes, ipport);
    }
}
