/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktcpclienttool.h"

#include <QHostAddress>

#include "sakcompatibility.h"
#include "sakinterface.h"

SAKTcpClientTool::SAKTcpClientTool(QObject* parent)
    : SAKSocketClientTool{parent}
{}

bool SAKTcpClientTool::initialize(QString& errStr)
{
    mTcpSocket = new QTcpSocket();
    if (mSpecifyClientIpPort) {
        if (!mTcpSocket->bind(QHostAddress(mClientIp), mClientPort)) {
            errStr = "Binding error: " + mTcpSocket->errorString();
            return false;
        }
    }

    mTcpSocket->connectToHost(QHostAddress(mServerIp), mServerPort);
    if (!mTcpSocket->waitForConnected()) {
        errStr = "Connect to host error: " + mTcpSocket->errorString();
        return false;
    }

    QString ip = mTcpSocket->localAddress().toString();
    QString port = QString::number(mTcpSocket->localPort());
    mBindingIpPort = ip + ":" + port;
    qInfo() << "Client address and port: " + mBindingIpPort;
    emit bindingIpPortChanged();

    connect(mTcpSocket, SAK_SIG_SOCKETERROROCCURRED, mTcpSocket, [=]() {
        QString info = "Error occurred: " + mTcpSocket->errorString();
        qInfo() << info;
        emit errorOccured(mTcpSocket->errorString());
    });

    connect(mTcpSocket, &QTcpSocket::disconnected, mTcpSocket, [=]() { exit(); });

    connect(mTcpSocket, &QTcpSocket::readyRead, mTcpSocket, [=]() { readBytes(); });

    return true;
}

void SAKTcpClientTool::writeBytes(const QByteArray& bytes)
{
    qint64 ret = mTcpSocket->write(bytes);
    if (ret == -1) {
        qWarning() << mTcpSocket->errorString();
    } else {
        emit bytesWritten(bytes, mBindingIpPort);
    }
}

void SAKTcpClientTool::uninitialize()
{
    mTcpSocket->disconnectFromHost();
    mTcpSocket->close();
    mTcpSocket->deleteLater();
    mTcpSocket = nullptr;
}

void SAKTcpClientTool::readBytes()
{
    QHostAddress address = mTcpSocket->peerAddress();
    quint16 port = mTcpSocket->peerPort();
    QByteArray bytes = mTcpSocket->readAll();
    if (!bytes.isEmpty()) {
        QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
        QString ipport = address.toString() + ":" + QString::number(port);
        QString info = mBindingIpPort + "<-" + ipport + ":";
        info += QString::fromLatin1(ba);
        qInfo() << info;
        emit outputBytes(bytes);
        emit bytesWritten(bytes, ipport);
    }
}
