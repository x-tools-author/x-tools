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

#include <QApplication>

#include "TcpSAKIODevice.h"

TcpSAKIODevice::TcpSAKIODevice(QString deviceName, QObject *parent):
    SAKIODevice(deviceName, parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

TcpSAKIODevice::~TcpSAKIODevice()
{

}

bool TcpSAKIODevice::isOpen()
{
    if (mpTcpSocket->state() != QTcpSocket::UnconnectedState){
        return true;
    }else {
        return false;
    }
}

void TcpSAKIODevice::readBytes()
{
    while (mpTcpSocket->waitForReadyRead(readDelayTime()));
    QByteArray datagram;
    datagram = mpTcpSocket->readAll();
    emit bytesRead(datagram);
}

void TcpSAKIODevice::writeBytes(QByteArray data)
{
    while (mpTcpSocket->waitForBytesWritten(writeDelayTime()));

    qint64 ret = mpTcpSocket->write(data);
    if (ret == -1){
        emit errorStr(mpTcpSocket->errorString());
    }else {
        emit bytesWritten(data);
    }

}

void TcpSAKIODevice::open(QString localhostAddress, QString LocalhostPort, QString serverAddress, QString serverPort)
{
    if (!localhostAddress.isEmpty()){
        if (mpTcpSocket->bind(QHostAddress(localhostAddress), quint16(LocalhostPort.toInt()))){

        }else {
            QApplication::beep();
            emit errorStr(QString("%1:%2").arg(tr("绑定地址端口失败")).arg(mpTcpSocket->errorString()));
            return;
        }
    }

    mpTcpSocket->connectToHost(QHostAddress(serverAddress), quint16(serverPort.toInt()));
    if (mpTcpSocket->waitForConnected()){
        if (mpTcpSocket->open(QTcpSocket::ReadWrite)){
            emit deviceOpenSuccessfully();
        }else{
            QApplication::beep();
            emit errorStr(QString("%1:%2").arg(tr("打开tcp套接字失败")).arg(mpTcpSocket->errorString()));
        }
    }else {
        QApplication::beep();
        emit errorStr(QString("%1:%2").arg(tr("无法连接到服务器")).arg(mpTcpSocket->errorString()));
    }
}

void TcpSAKIODevice::close()
{
    mpTcpSocket->disconnectFromHost();

    if (mpTcpSocket->state() != QTcpSocket::UnconnectedState){
        if (mpTcpSocket->waitForDisconnected()){
            mpTcpSocket->close();
            emit deviceCloseSuccessfully();
        }
    }else {
        mpTcpSocket->close();
        emit deviceCloseSuccessfully();
    }
}

void TcpSAKIODevice::afterDisConnect()
{
    mpTcpSocket->close();
    emit errorStr("与服务器断开断开连接！");
    emit deviceCloseSuccessfully();
}

void TcpSAKIODevice::resetTheDevice()
{
    delete mpTcpSocket;
    mpTcpSocket = new QTcpSocket;
    connect(mpTcpSocket, SIGNAL(readyRead()), this, SLOT(readBytes()));
    emit infoStr(tr("重置设备成功！"));
}

void TcpSAKIODevice::run()
{
    mpTcpSocket = new QTcpSocket;
    connect(mpTcpSocket, SIGNAL(readyRead()), this, SLOT(readBytes()));
    connect(mpTcpSocket, SIGNAL(disconnected()), this, SLOT(afterDisConnect()));
    this->exec();
}
