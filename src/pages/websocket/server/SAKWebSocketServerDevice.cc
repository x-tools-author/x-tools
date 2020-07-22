/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKWebSocketServerDevice.hh"
#include "SAKWebSocketServerDebugPage.hh"
#include "SAKWebSocketServerDeviceController.hh"

SAKWebSocketServerDevice::SAKWebSocketServerDevice(SAKWebSocketServerDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,mDebugPage(debugPage)
    ,mWebSocketServer(Q_NULLPTR)
{

}

void SAKWebSocketServerDevice::run()
{
    QEventLoop eventLoop;
    SAKWebSocketServerDeviceController *deviceController = mDebugPage->controllerInstance();
    mServerHost = deviceController->serverHost();
    mServerPort = deviceController->serverPort();
    QString serverName = QString("%1:%2").arg(mServerHost).arg(mServerPort);

    QList<QWebSocket*> clientList;
    mWebSocketServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(mServerHost), mServerPort)){
        emit deviceStateChanged(false);
        emit messageChanged(tr("服务器监听地址、端口失败：")+mWebSocketServer->errorString(), false);
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
        while (mWebSocketServer->hasPendingConnections()){
            QWebSocket *socket = mWebSocketServer->nextPendingConnection();
            if (socket){
                clientList.append(socket);
                deviceController->addClient(socket->peerAddress().toString(), socket->peerPort(), socket);
                /// @brief 读数据
                connect(socket, &QWebSocket::textMessageReceived, [&](const QString message){
                    innerReadBytes(socket, message.toLatin1(), deviceController);
                });
                /// @brief 读数据
                connect(socket, &QWebSocket::binaryMessageReceived, [&](const QByteArray message){
                    innerReadBytes(socket, message, deviceController);
                });
                /// @brief 断开连接后移除先关数据
                connect(socket, &QWebSocket::disconnected, [&](){
                    deviceController->removeClient(socket);
                    clientList.removeOne(socket);
                });
            }
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
        mThreadMutex.lock();
        mThreadWaitCondition.wait(&mThreadMutex, mDebugPage->readWriteParameters().runIntervalTime);
        mThreadMutex.unlock();
    }

    mWebSocketServer->close();
    delete mWebSocketServer;
    deviceController->clearClient();
    emit deviceStateChanged(false);
}

void SAKWebSocketServerDevice::innerReadBytes(QWebSocket *socket, QByteArray bytes, SAKWebSocketServerDeviceController *deviceController)
{        
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

void SAKWebSocketServerDevice::innerWriteBytes(QWebSocket *socket, QByteArray bytes, SAKWebSocketServerDeviceController *deviceController)
{    
    QString currentClientHost = deviceController->currentClientHost();
    QString peerHost = socket->peerAddress().toString();
    quint16 currentClientPort = deviceController->currentClientPort();
    quint16 peerPort = socket->peerPort();
    if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
        qint64 ret = 0;
        if (deviceController->sendingType() == SAKDataStruct::WebSocketSendingTypeText){
            ret = socket->sendTextMessage(QString(bytes));
        }else{
            ret = socket->sendBinaryMessage(bytes);
        }

        if (ret == -1){
            emit messageChanged(tr("无法写入数据:(%1)%2").arg(socket->peerAddress().toString().arg(socket->error())), false);
        }else{
            emit bytesWritten(bytes);
        }
    }
}
