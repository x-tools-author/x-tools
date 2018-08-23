/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "UdpSAKIODevice.h"

UdpSAKIODevice::UdpSAKIODevice(SAKDeviceType deviceType, QObject *parent):
    SAKIODevice(deviceType, parent)
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
    quint16 peerPort;
    int ret;
    while (mpUdpSocket->hasPendingDatagrams()){
        datagram.resize(mpUdpSocket->pendingDatagramSize());
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
    qint64 ret = mpUdpSocket->writeDatagram(data, QHostAddress(peerHostAddress), quint16(peerHostPort.toInt()));
    if (ret == -1){
        emit errorStr(mpUdpSocket->errorString());
    }else {
        emit bytesWritten(ret);
    }
}

void UdpSAKIODevice::open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort)
{
    peerHostAddress = peerAddress;
    peerHostPort = peerPort;

    if (mpUdpSocket->bind(QHostAddress(hostAddress), (qint16)hostPort.toInt())){
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
