/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugPage.hh"
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTcpServerDeviceController.hh"

SAKTcpServerDevice::SAKTcpServerDevice(SAKTcpServerDebugPage *debugPage, QObject *parent)
    :SAKDevice(parent)
    ,debugPage(debugPage)
    ,tcpServer(Q_NULLPTR)
{

}

void SAKTcpServerDevice::run()
{
    QEventLoop eventLoop;
    SAKTcpServerDeviceController *deviceController = debugPage->controllerInstance();
    serverHost = deviceController->serverHost();
    serverPort = deviceController->serverPort();

    QList<QTcpSocket*> clientList;
    tcpServer = new QTcpServer;
    if (!tcpServer->listen(QHostAddress(serverHost), serverPort)){
        emit deviceStateChanged(false);
        emit messageChanged(tr("服务器监听地址、端口失败：")+tcpServer->errorString(), false);
        return;
    }else{
        emit deviceStateChanged(true);
    }

    while (true){
        /// @brief 响应中断
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 处理接入
        while (tcpServer->hasPendingConnections()){
            QTcpSocket *socket = tcpServer->nextPendingConnection();
            if (socket){
                clientList.append(socket);
                deviceController->addClient(socket->peerAddress().toString(), socket->peerPort(), socket);
            }
        }

        /// @brief 检查链接状态，移除已断开的链接
        for (auto var : clientList){
            QList<QTcpSocket*> offLineClientList;
            if (var->state() != QTcpSocket::ConnectedState){
                offLineClientList.append(var);
            }

            for (auto var : offLineClientList){
                /// @brief socket（127.0.0.1）断开链接后无法获取：peerAddress及eerPort
                deviceController->removeClient(var);
                clientList.removeOne(var);
            }
        }

        /// @brief 读取数据
        for (auto var : clientList){
            innerReadBytes(var, deviceController);
        }

        /// @brief 写数据
        while (true){
            QByteArray bytes = takeWaitingForWrittingBytes();
            if (bytes.length()){
                for (auto var : clientList){
                    innerWriteBytes(var, bytes, deviceController);
                }
            }else{
                break;
            }
        }

        /// @brief 处理线程事件
        eventLoop.processEvents();

        /// @brief 线程睡眠
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, debugPage->readWriteParameters().runIntervalTime);
        threadMutex.unlock();
    }

    tcpServer->close();
    delete tcpServer;
    emit deviceStateChanged(false);
}

void SAKTcpServerDevice::innerReadBytes(QTcpSocket *socket, SAKTcpServerDeviceController *deviceController)
{        
    socket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
    QByteArray bytes = socket->readAll();
    QString currentClientHost = deviceController->currentClientHost();
    QString peerHost = socket->peerAddress().toString();
    quint16 currentClientPort = deviceController->currentClientPort();
    quint16 peerPort = socket->peerPort();

    if (bytes.length()){
        if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
            emit bytesRead(bytes);
        }
    }
}

void SAKTcpServerDevice::innerWriteBytes(QTcpSocket *socket, QByteArray bytes, SAKTcpServerDeviceController *deviceController)
{    
    QString currentClientHost = deviceController->currentClientHost();
    QString peerHost = socket->peerAddress().toString();
    quint16 currentClientPort = deviceController->currentClientPort();
    quint16 peerPort = socket->peerPort();
    if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
        qint64 ret = socket->write(bytes);
        socket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
        if (ret == -1){
            emit messageChanged(tr("无法写入数据:(%1)%2").arg(socket->peerAddress().toString().arg(socket->error())), false);
        }else{
            emit bytesWritten(bytes);
        }
    }
}
