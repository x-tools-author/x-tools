/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QTcpSocket>
#include "EDTcpServerTool.hpp"

EDTcpServerTool::EDTcpServerTool(QObject *parent)
    : EDBaseTool{"ED.TcpServerTool", parent}
{

}

bool EDTcpServerTool::initialize(QString &errStr)
{
    mTcpServer = new QTcpServer();
    if (!mTcpServer->listen(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mTcpServer->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mServerIp).arg(mServerPort);
    outputMessage(QtInfoMsg, info);
    setToolFlag(info);

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
            emit bytesOutputted(ipPort, bytes);
        });

        connect(client, &QTcpSocket::disconnected, client, [=](){
            this->mTcpSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });

        connect(client, &QTcpSocket::errorOccurred, client, [=](QAbstractSocket::SocketError err){
            this->mTcpSocketList.removeOne(client);
            this->mClients.removeOne(ipPort);
            emit clientsChanged();
        });
    });

    return true;
}

void EDTcpServerTool::inputBytesHandler(const QByteArray &bytes)
{
    if (mClientIndex >= 0 && mClientIndex < mTcpSocketList.length()) {
        QTcpSocket *socket = mTcpSocketList.at(mClientIndex);
        qint64 ret = socket->write(bytes);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mTcpServer->errorString());
        } else {
            emit bytesInputted(toolFlag(), bytes);
        }
    }
}

void EDTcpServerTool::outputBytesHandler()
{
    // Nothing to do yet.
}

void EDTcpServerTool::uninitialize()
{
    mTcpServer->close();
    mTcpServer->deleteLater();
    mTcpServer = nullptr;
}
