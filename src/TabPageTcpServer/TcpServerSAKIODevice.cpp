/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "TcpServerSAKIODevice.h"

TcpServerSAKIODevice::TcpServerSAKIODevice(QString deviceName, QObject *parent):
    SAKIODevice(deviceName, parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

TcpServerSAKIODevice::~TcpServerSAKIODevice()
{

}

void TcpServerSAKIODevice::readBytes()
{
    if ((currentTcpClient == nullptr) || (currentTcpClient == nullptr)){
        emit errorStr(UI_STR_DEVICE_IS_NULL);
    }else {
        while (currentTcpClient->waitForReadyRead(readDelayTime()));
        QByteArray data;
        data = currentTcpClient->readAll();
        emit bytesRead(data);
    }
}

void TcpServerSAKIODevice::writeBytes(QByteArray data)
{
    if ((currentTcpClient == nullptr) || (currentTcpClient == nullptr)){
        emit errorStr(UI_STR_DEVICE_IS_NULL);
    }else {
        while(currentTcpClient->waitForBytesWritten(writeDelayTime()));
        qint64 ret = currentTcpClient->write(data);
        if (ret == -1){
            emit errorStr(currentTcpClient->errorString());
        }else {
            emit bytesWritten(data);
        }
    }
}

void TcpServerSAKIODevice::open(QString serverAddress, QString serverPort)
{
    if (mpTcpServer == nullptr || mpTcpServer == nullptr){
        emit errorStr(UI_STR_DEVICE_IS_NULL);
    }else {
        if (mpTcpServer->listen(QHostAddress(serverAddress), quint16(serverPort.toInt()))){
            emit deviceOpenSuccessfully();
        }else{
            emit errorStr(currentTcpClient->errorString());
        }
    }
}

void TcpServerSAKIODevice::close()
{
    mpTcpServer->close();
    emit deviceCloseSuccessfully();
    foreach (QTcpSocket *client, clients) {
        client->close();
        delete client;
    }

    currentTcpClient = nullptr;
    clients.clear();
    emit clientsChanged(clients);
}

void TcpServerSAKIODevice::run()
{
    qRegisterMetaType<QList <QTcpSocket *>>();

    currentTcpClient = nullptr;
    mpTcpServer = new QTcpServer;
    connect(mpTcpServer, SIGNAL(newConnection()), this, SLOT(newConnecting()));

    stateTimer = new QTimer;
    connect(stateTimer, SIGNAL(timeout()), this, SLOT(checkState()));
    stateTimer->start(2*1000);

    this->exec();
}

void TcpServerSAKIODevice::newConnecting()
{
    QTcpSocket *newTcpSocket = mpTcpServer->nextPendingConnection();
    if (clients.isEmpty()){
        currentTcpClient = newTcpSocket;
        connect(currentTcpClient, SIGNAL(readyRead()), this, SLOT(readBytes()));
    }
    clients.append(newTcpSocket);
    emit clientsChanged(clients);
}

void TcpServerSAKIODevice::checkState()
{
    if (clients.isEmpty()){
        return;
    }else {
        QList <QTcpSocket *> clientsTemp = clients;
        int index = 0;
        bool need2updateClients = false;
        foreach (QTcpSocket *client, clientsTemp) {
            if (client->state() == QTcpSocket::UnconnectedState){
                if ((client->peerAddress().toString().compare(currentTcpClient->peerAddress().toString()) == 0) && (client->peerPort() == currentTcpClient->peerPort())){
                    need2updateClients = true;
                    clients.removeAt(index);
                    delete client;
                    currentTcpClient = nullptr;
                }else {
                    need2updateClients = true;
                    clients.removeAt(index);
                    delete client;
                }
            }
            index += 1;
        }

        if (need2updateClients){
            emit clientsChanged(clients);
        }
    }
}

void TcpServerSAKIODevice::changedCurrentScoket(QString address, QString port)
{
    disconnect(currentTcpClient, SIGNAL(readyRead()), this, SLOT(readBytes()));
    foreach(QTcpSocket *client, clients){
        if ((client->peerAddress().toString().compare(address) == 0) && (client->peerPort() == static_cast<quint16>(port.toInt()))){
            currentTcpClient = client;
            connect(currentTcpClient, SIGNAL(readyRead()), this, SLOT(readBytes()));
        }
    }
}
