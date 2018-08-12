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

#include "TCPClientAssistant.h"

TcpClientAssistant::TcpClientAssistant(QObject *parent):
    QThread(parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

TcpClientAssistant::~TcpClientAssistant()
{

}

QByteArray TcpClientAssistant::ReadAll()
{
    QByteArray datagram;
    datagram = mpTcpSocket->readAll();
    emit NewDataHadBeenRead(datagram);
    return datagram;
}

qint64 TcpClientAssistant::Write(QByteArray data)
{
    qint64 ret = mpTcpSocket->write(data);
    if (ret == -1){
        mErrorString = mpTcpSocket->errorString();
    }else {
        mErrorString = "No error!";
        emit NewDataHadBeenWrite(data);
    }
    return ret;
}

void TcpClientAssistant::Open(QString localhostAddress, QString LocalhostPort, QString serverAddress, QString serverPort)
{
    if (mpTcpSocket->isOpen()){
        mpTcpSocket->close();
    }else {
        if (LocalhostPort.isEmpty()){
            LocalhostPort = "0";
        }

        if (mpTcpSocket->bind(QHostAddress(localhostAddress), quint16(LocalhostPort.toInt()))){
            mpTcpSocket->connectToHost(QHostAddress(serverAddress), quint16(serverPort.toInt()));
            if (mpTcpSocket->waitForConnected()){
                if (mpTcpSocket->open(QTcpSocket::ReadWrite)){
                    emit TcpSocketOpenSuccessfully();
                }else{
                    qWarning() << tr("打开tcp套接字失败：") << mpTcpSocket->errorString();
                }
            }else {
                qWarning() << tr("无法连接到服务器：") << mpTcpSocket->errorString();
            }
        }else {
            qWarning() << tr("指定本地地址和端口失败：") << mpTcpSocket->errorString();
        }
    }
}

void TcpClientAssistant::Close()
{
    mpTcpSocket->disconnectFromHost();
    if (mpTcpSocket->waitForDisconnected()){

    }else {

    }
    mpTcpSocket->close();
    emit TcpSocketCloseSuccessfully();
}

void TcpClientAssistant::Disconnect()
{
    Close();
    qWarning() << tr("与服务器断开连接！");
}

void TcpClientAssistant::run()
{
    mpTcpSocket = new QTcpSocket;
    connect(mpTcpSocket, SIGNAL(readyRead()), this, SLOT(ReadAll()));
    connect(mpTcpSocket, SIGNAL(disconnected()), this, SLOT(Disconnect()));
    this->exec();
}
