/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugPage.hh"
#include "SAKTcpServerDevice.hh"

SAKTcpServerDevice::SAKTcpServerDevice(QString targetHost, quint16 targetPort,
                           SAKDebugPage *debugPage,
                           QObject *parent)
    :QThread (parent)
    ,serverHost (targetHost)
    ,serverPort (targetPort)
    ,debugPage (debugPage)
{
    moveToThread(this);
}

void SAKTcpServerDevice::run()
{
    tcpServer = new QTcpServer(this);

    connect(qApp, &QApplication::lastWindowClosed, this, &SAKTcpServerDevice::terminate);
    connect(tcpServer, &QTcpServer::newConnection, this, &SAKTcpServerDevice::newConnection);

    if (!tcpServer->listen(QHostAddress(serverHost), serverPort)){
        emit deviceStatuChanged(false);
        emit messageChanged(tr("服务器监听地址、端口失败：")+tcpServer->errorString(), false);
    }else{
        emit deviceStatuChanged(true);
        exec();
    }
}

void SAKTcpServerDevice::afterDisconnected()
{
    emit deviceStatuChanged(false);
    emit messageChanged(tr("客户端已断开"), false);
}

void SAKTcpServerDevice::newConnection()
{
    while (1) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        if (socket){
            clients.append(socket);
            connect(socket, &QTcpSocket::readyRead, this, &SAKTcpServerDevice::readBytes);
            connect(socket, &QTcpSocket::disconnected, this, &SAKTcpServerDevice::afterDisconnected);

            emit newClientConnected(socket->peerAddress().toString(), socket->peerPort());
        }else {
            return;
        }
    }
}

void SAKTcpServerDevice::readBytes()
{        
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    socket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
    QByteArray data = socket->readAll();
    if (!data.isEmpty()){
        emit bytesRead(data, socket->peerAddress().toString(), socket->peerPort());
    }
}

void SAKTcpServerDevice::writeBytes(QByteArray data, QString host, quint16 port)
{    
    for(auto var:clients){
        if ((var->peerAddress().toString().compare(host) == 0) && (var->peerPort() == port)){
            qint64 ret = var->write(data);
            if (ret == -1){
                emit messageChanged(tr("无法写入数据:(%1)%2").arg(var->peerAddress().toString().arg(var->error())), false);
            }else{
                emit bytesWritten(data, var->peerAddress().toString(), var->peerPort());
            }
        }
    }
}
