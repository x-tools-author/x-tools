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
#include "SAKTcpClientDevice.hh"

SAKTcpClientDevice::SAKTcpClientDevice(QString localHost, quint16 localPort,
                           bool enableCustomLocalSetting,
                           QString targetHost, quint16 targetPort,
                           SAKDebugPage *debugPage,
                           QObject *parent)
    :QThread (parent)
    ,localHost (localHost)
    ,localPort (localPort)
    ,enableCustomLocalSetting (enableCustomLocalSetting)
    ,serverHost (targetHost)
    ,serverPort (targetPort)
    ,debugPage (debugPage)
{
    moveToThread(this);
}

void SAKTcpClientDevice::run()
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &SAKTcpClientDevice::readBytes);
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKTcpClientDevice::terminate);

    bool bindResult = false;
    if (enableCustomLocalSetting){
        bindResult = tcpSocket->bind(QHostAddress(localHost), localPort);
    }else{
        bindResult = tcpSocket->bind();
    }

    if (bindResult){
        tcpSocket->connectToHost(serverHost, serverPort);
        if (tcpSocket->waitForConnected()){
            if (tcpSocket->open(QTcpSocket::ReadWrite)){
#ifdef QT_DEBUG
            qDebug() << tcpSocket->localAddress().toString() << tcpSocket->localPort();
#endif
                emit deviceStatuChanged(true);
                exec();
            }else{
                emit deviceStatuChanged(false);
                emit messageChanged(tr("无法打开设备")+tcpSocket->errorString(), false);
            }
        }else{
            emit deviceStatuChanged(false);
            emit messageChanged(tr("无法连接到服务器")+tcpSocket->errorString(), false);
        }
    }else{
        emit deviceStatuChanged(false);
        emit messageChanged(tr("无法绑定设备")+tcpSocket->errorString(), false);
    }

    exec();
}

void SAKTcpClientDevice::afterDisconnected()
{
    emit deviceStatuChanged(false);
    emit messageChanged(tr("服务器已断开"), false);
}

void SAKTcpClientDevice::readBytes()
{        
    tcpSocket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
    QByteArray data = tcpSocket->readAll();
    if (!data.isEmpty()){
        emit bytesRead(data);
    }
}

void SAKTcpClientDevice::writeBytes(QByteArray data)
{    
    qint64 ret = tcpSocket->write(data);
    tcpSocket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
    if (ret == -1){
        emit messageChanged(tr("发送数据失败：")+tcpSocket->errorString(), false);
    }else{
        emit bytesWriten(data);
    }
}
