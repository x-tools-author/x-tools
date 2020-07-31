/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QWebSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include "SAKDataStruct.hh"
#include "SAKWebSocketClientDevice.hh"
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketClientDeviceController.hh"

SAKWebSocketClientDevice::SAKWebSocketClientDevice(SAKWebSocketClientDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,debugPage (debugPage)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}

void SAKWebSocketClientDevice::run()
{
    QEventLoop eventLoop;
    SAKWebSocketClientDeviceController *deviceController = debugPage->controllerInstance();
    QString serverAddress = deviceController->serverAddress();

    QWebSocket *webSocket = new QWebSocket;
    connect(webSocket, &QWebSocket::connected, [&](){
        emit deviceStateChanged(true);
    });

    connect(webSocket, &QWebSocket::textMessageReceived, [&](QString message){
        emit bytesRead(message.toLatin1());
    });

    connect(webSocket, &QWebSocket::disconnected, [&](){
        requestInterruption();
    });

    /// @brief 错误处理
    connect(webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSlot(QAbstractSocket::SocketError)));

    webSocket->open(serverAddress);

    while (true) {
        /// @brief 处理外部中断请求
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 发送数据
        while (true) {
            QByteArray bytes = takeWaitingForWrittingBytes();
            if (bytes.length()){
                if (webSocket->state() == QAbstractSocket::ConnectedState){
                    qint64 ret = 0;
                    if (deviceController->sendingType() == SAKDataStruct::WebSocketSendingTypeText){
                        ret = webSocket->sendTextMessage(QString(bytes));
                    }else{
                        ret = webSocket->sendBinaryMessage(bytes);
                    }

                    if (ret != -1){
                        emit bytesWritten(bytes);
                    }
                }
            }else{
                break;
            }
        }

        /// @brief 处理事件循环
        eventLoop.processEvents();

        /// @brief 线程睡眠
        if (isInterruptionRequested()){
            mThreadMutex.lock();
            mThreadWaitCondition.wait(&mThreadMutex, 50);
            mThreadMutex.unlock();
        }
    }

    webSocket->close();
    webSocket->deleteLater();
    webSocket = Q_NULLPTR;
    emit deviceStateChanged(false);
}

void SAKWebSocketClientDevice::errorSlot(QAbstractSocket::SocketError error)
{
    messageChanged(tr("连接错误：%1").arg(error), false);
    requestInterruption();
}
