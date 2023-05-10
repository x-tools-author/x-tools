/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTcpSocket>
#include "SAKTcpServerTool.hh"

SAKTcpServerTool::SAKTcpServerTool(QObject *parent)
    : SAKCommunicationTool{"SAK.TcpServerTool", parent}
{

}

bool SAKTcpServerTool::initialize()
{
    mTcpServer = new QTcpServer();
    if (!mTcpServer->listen(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mTcpServer->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    outputMessage(QtInfoMsg, info);

    connect(mTcpServer, &QTcpServer::newConnection, mTcpServer, [=](){
        QTcpSocket *client = mTcpServer->nextPendingConnection();
        mTcpSocketList.append(client);

        QString ip = client->peerAddress().toString();
        quint16 port = client->peerPort();
        QString ipPort = QString("%1:%2").arg(ip).arg(port);
        mClients.append(ipPort);
        qDebug() << "111" << ipPort;
        emit clientsChanged();

        connect(client, &QTcpSocket::readyRead, client, [=](){
            QByteArray bytes = client->readAll();
            emit bytesOutputted(bytes, QVariant());
        });

        connect(client, &QTcpSocket::disconnected, client, [=](){
            this->mTcpSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });

        connect(client, &QTcpSocket::errorOccurred, client, [=](QAbstractSocket::SocketError err){
            Q_UNUSED(err);
            this->mTcpSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });
    });

    mIsTimerReading = false;
    return true;
}

void SAKTcpServerTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    Q_UNUSED(context);
    if (mClientIndex >= 0 && mClientIndex < mTcpSocketList.length()) {
        QTcpSocket *socket = mTcpSocketList.at(mClientIndex);
        qint64 ret = socket->write(bytes);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mTcpServer->errorString());
        } else {
            emit bytesInputted(bytes, QVariant());
        }
    }
}

void SAKTcpServerTool::readBytes()
{
    // Nothing to do yet.
}

void SAKTcpServerTool::uninitialize()
{
    mTcpServer->close();
    mTcpServer->deleteLater();
    mTcpServer = nullptr;
}
