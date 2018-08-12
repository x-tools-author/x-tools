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

#include "TCPServerAssistant.h"

TcpServerAssistant::TcpServerAssistant(QObject *parent):
    QThread(parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

TcpServerAssistant::~TcpServerAssistant()
{

}

QByteArray TcpServerAssistant::ReadAll()
{
    if (mpCurrentTcpClient == NULL){
        return QByteArray("");
    }

    QByteArray data;

    data = mpCurrentTcpClient->readAll();
    emit NewDataHadBeenRead(data);

    return data;
}

qint64 TcpServerAssistant::Write(QByteArray data)
{
    if (mpCurrentTcpClient == NULL){
        return -1;
    }
    qint64 ret = mpCurrentTcpClient->write(data);
    if (ret == -1){
        mErrorString = mpCurrentTcpClient->errorString();
    }else {
        mErrorString = "No error!";
        emit NewDataHadBeenWrite(data);
    }
    return ret;
}

void TcpServerAssistant::Open(QString serverAddress, QString serverPort)
{
    if (mpTcpServer->isListening()){
        mpTcpServer->close();
    }else {
        if (mpTcpServer->listen(QHostAddress(serverAddress), quint16(serverPort.toInt()))){
            emit TcpServerOpenSuccessfully();
        }
    }
}

void TcpServerAssistant::Close()
{
    mpTcpServer->close();
    emit TcpServerCloseSuccessfully();
}

void TcpServerAssistant::Disconnect()
{
    Close();
    qWarning() << tr("与服务器断开连接！");
}

void TcpServerAssistant::NewConnect()
{
    QTcpSocket *pTcpSocket = mpTcpServer->nextPendingConnection();
    mTcpClientList.append(pTcpSocket);
    QString str;
    for (int i = 0; i <mTcpClientList.length(); i++){
        str = mTcpClientList.at(i)->peerAddress().toString() + ":" + QString::number(mTcpClientList.at(i)->peerPort());
    }

    if (mTcpClientList.length() == 1){
        mpCurrentTcpClient = pTcpSocket;
        connect(mpCurrentTcpClient, SIGNAL(readyRead()), this, SLOT(ReadAll()));
    }

    emit UpdateTcpClientList(mTcpClientList);
}

void TcpServerAssistant::UpdateCurrentTcpClient(QHostAddress address, quint16 port)
{
    for (int i = 0; i < mTcpClientList.length(); i++){
        disconnect(mTcpClientList.at(i), SIGNAL(readyRead()), this, SLOT(ReadAll()));
        if ((mTcpClientList.at(i)->peerAddress() == address) && (mTcpClientList.at(i)->peerPort() == port)){
            mpCurrentTcpClient = mTcpClientList.at(i);
            connect(mpCurrentTcpClient, SIGNAL(readyRead()), this, SLOT(ReadAll()));
        }
    }
}

void TcpServerAssistant::CheckStateTimerTimeout()
{
    if (mTcpClientList.isEmpty()){
        return;
    }else {
        for (int i = 0; i < mTcpClientList.length(); i++){
            if (mTcpClientList.at(i)->state() == QTcpSocket::UnconnectedState){
                if (mpCurrentTcpClient == mTcpClientList.at(i)){
                    mpCurrentTcpClient = NULL;
                    delete mTcpClientList.at(i);
                    mTcpClientList.removeAt(i);
                }else {
                    delete mTcpClientList.at(i);
                    mTcpClientList.removeAt(i);
                }
            }
        }
        emit UpdateTcpClientList(mTcpClientList);
    }
}

void TcpServerAssistant::run()
{
    mpTcpServer = new QTcpServer;
    connect(mpTcpServer, SIGNAL(newConnection()), this, SLOT(NewConnect()));

    qRegisterMetaType<QList <QTcpSocket *>>();

    mpCheckStateTimer = new QTimer;
    connect(mpCheckStateTimer, SIGNAL(timeout()), this, SLOT(CheckStateTimerTimeout()));
    mpCheckStateTimer->start(2*1000);

    this->exec();
}
