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

#include "UdpSAKIODevice.h"

UdpSAKIODevice::UdpSAKIODevice(QString deviceName, QObject *parent):
    SAKIODevice(deviceName, parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

UdpSAKIODevice::~UdpSAKIODevice()
{

}

void UdpSAKIODevice::readBytes()
{
    QByteArray datagram;

    QHostAddress peerHostAddress;
    quint16 peerPort = 0;
    qint64 ret;
    while(mpUdpSocket->waitForReadyRead(readDelayTime()));
    while (mpUdpSocket->hasPendingDatagrams()){
        datagram.resize(static_cast<int>(mpUdpSocket->pendingDatagramSize()));
        ret = mpUdpSocket->readDatagram(datagram.data(), datagram.length(), &peerHostAddress, &peerPort);
        if (ret != -1){
            emit bytesRead(datagram);
        }else {
            emit errorStr(mpUdpSocket->errorString());
        }
    }
    emit peerHostInfo(peerHostAddress.toString(), peerPort);
}

void UdpSAKIODevice::writeBytes(QByteArray data)
{
    while (mpUdpSocket->waitForBytesWritten(writeDelayTime()));
    qint64 ret = mpUdpSocket->writeDatagram(data, QHostAddress(peerHostAddress), quint16(peerHostPort.toInt()));
    if (ret == -1){
        emit errorStr(mpUdpSocket->errorString());
    }else {
        emit bytesWritten(data);
    }
}

void UdpSAKIODevice::open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort)
{
    peerHostAddress = peerAddress;
    peerHostPort = peerPort;

    if (mpUdpSocket->bind(QHostAddress(hostAddress), static_cast<quint16>(hostPort.toInt()))){
        if (mpUdpSocket->open(QUdpSocket::ReadWrite)){
            emit deviceOpenSuccessfully();
        }else{
            emit errorStr(mpUdpSocket->errorString());
        }
    }else {
        emit errorStr(mpUdpSocket->errorString());
    }
}

void UdpSAKIODevice::close()
{
    mpUdpSocket->close();
    emit deviceCloseSuccessfully();
}

void UdpSAKIODevice::run()
{
    mpUdpSocket = new QUdpSocket;
    connect(mpUdpSocket, SIGNAL(readyRead()), this, SLOT(readBytes()));

    this->exec();
}
