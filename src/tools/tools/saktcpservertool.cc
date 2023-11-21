/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QTcpSocket>

#include "sakcompatibility.h"
#include "saktcpservertool.h"

SAKTcpServerTool::SAKTcpServerTool(QObject *parent)
    : SAKSocketServerTool{"sak.tcpservertool", parent}
{}

bool SAKTcpServerTool::initialize(QString &errStr)
{
    mTcpServer = new QTcpServer();
    if (!mTcpServer->listen(QHostAddress(mServerIp), mServerPort)) {
        errStr = "Error occurred: " + mTcpServer->errorString();
        return false;
    }

    mBindingIpPort = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    outputMessage(QtInfoMsg, "Server ip and port: " + mBindingIpPort);

    connect(mTcpServer, &QTcpServer::newConnection, mTcpServer, [=]() {
        QTcpSocket *client = mTcpServer->nextPendingConnection();
        this->mTcpSocketListMutex.lock();
        this->mTcpSocketList.append(client);
        this->mTcpSocketListMutex.unlock();

        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        mClients.append(ipPort);
        outputMessage(QtInfoMsg, "New connection:" + ipPort);
        emit clientsChanged();

        connect(client, &QTcpSocket::readyRead, client, [=]() {
            QByteArray bytes = client->readAll();
            QString hex = bytes.toHex();
            outputMessage(QtInfoMsg, QString("%1<-%2:%3").arg(mBindingIpPort, ipPort, hex));
            emit bytesOutputted(bytes, QVariant());
        });

        connect(client, &QTcpSocket::disconnected, client, [=]() {
            client->deleteLater();
            this->mTcpSocketListMutex.lock();
            this->mTcpSocketList.removeOne(client);
            this->mTcpSocketListMutex.unlock();
            this->mClients.removeOne(ipPort);
            outputMessage(QtInfoMsg,
                          QString("Connection(%1) has been disconnected: %2")
                              .arg(ipPort, client->errorString()));
            emit clientsChanged();
        });

        connect(client, SAK_SIG_SOCKETERROROCCURRED, client, [=]() {
            this->mTcpSocketListMutex.lock();
            this->mTcpSocketList.removeOne(client);
            this->mTcpSocketListMutex.unlock();
            this->mClients.removeOne(ipPort);
            outputMessage(QtInfoMsg, QString("Error occurred: %1").arg(client->errorString()));
            emit clientsChanged();
        });
    });

    return true;
}

void SAKTcpServerTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    Q_UNUSED(context);
    if (mClientIndex >= 0 && mClientIndex < mTcpSocketList.length()) {
        QTcpSocket *client = mTcpSocketList.at(mClientIndex);
        writeBytesInner(client, bytes, context);
    } else {
        for (auto &client : mTcpSocketList) {
            writeBytesInner(client, bytes, context);
        }
    }
}

void SAKTcpServerTool::uninitialize()
{
    mTcpServer->close();
    mTcpServer->deleteLater();
    mTcpServer = nullptr;
}

void SAKTcpServerTool::writeBytesInner(QTcpSocket *client,
                                       const QByteArray &bytes,
                                       const QVariant &context)
{
    qint64 ret = client->write(bytes);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mTcpServer->errorString());
    } else {
        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        QString hex = bytes.toHex();
        outputMessage(QtInfoMsg, QString("%1->%2:%3").arg(mBindingIpPort, ipPort, hex));
        emit bytesInputted(bytes, context);
    }
}
