/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QEventLoop>
#include <QWebSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include "SAKWebSocketClientDevice.hh"
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketClientDeviceController.hh"

SAKWebSocketClientDevice::SAKWebSocketClientDevice(SAKWebSocketClientDebugPage *debugPage, QObject *parent)
    :SAKDevice(parent)
    ,debugPage (debugPage)
{

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
                    webSocket->sendTextMessage(QString(bytes));
                }
            }else{
                break;
            }
        }

        /// @brief 处理事件循环
        eventLoop.processEvents();

        /// @brief 线程睡眠
        if (isInterruptionRequested()){
            threadMutex.lock();
            threadWaitCondition.wait(&threadMutex, 50);
            threadMutex.unlock();
        }
    }

    webSocket->close();
    webSocket->deleteLater();
    webSocket = Q_NULLPTR;
}
