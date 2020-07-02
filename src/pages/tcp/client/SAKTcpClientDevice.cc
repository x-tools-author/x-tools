/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
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

#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDevice::SAKTcpClientDevice(SAKTcpClientDebugPage *debugPage, QObject *parent)
    :SAKDevice(parent)
    ,debugPage (debugPage)
{

}

void SAKTcpClientDevice::run()
{
    QEventLoop eventLoop;
    SAKTcpClientDeviceController *deviceController = debugPage->controllerInstance();
    localHost = deviceController->localHost();
    localPort = deviceController->localPort();
    serverHost = deviceController->serverHost();
    serverPort = deviceController->serverPort();
    enableCustomLocalSetting = deviceController->enableCustomLocalSetting();

    tcpSocket = new QTcpSocket;

    bool bindResult = false;
    if (enableCustomLocalSetting){
        bindResult = tcpSocket->bind(QHostAddress(localHost), localPort);
    }else{
        bindResult = tcpSocket->bind();
    }

    if (!bindResult){
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法绑定设备")+tcpSocket->errorString(), false);
        delete tcpSocket;
        return;
    }

    tcpSocket->connectToHost(serverHost, serverPort);
    if (tcpSocket->state() != QTcpSocket::ConnectedState){
        if (!tcpSocket->waitForConnected()){
            emit deviceStateChanged(false);
            emit messageChanged(tr("无法连接到服务器")+tcpSocket->errorString(), false);
            delete tcpSocket;
            return;
        }
    }


    if (tcpSocket->open(QTcpSocket::ReadWrite)){
        emit deviceStateChanged(true);

        while (true) {
            /// @brief 响应中断
            if (isInterruptionRequested()){
                break;
            }

            /// @brief 读取数据
            tcpSocket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
            QByteArray bytes = tcpSocket->readAll();
            if (!bytes.isEmpty()){
                emit bytesRead(bytes);
            }

            /// @brief 发送数据
            while (true){
                QByteArray bytes = takeWaitingForWrittingBytes();
                if (bytes.length()){
                    qint64 ret = tcpSocket->write(bytes);
                    tcpSocket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
                    if (ret == -1){
                        emit messageChanged(tr("发送数据失败：")+tcpSocket->errorString(), false);
                    }else{
                        emit bytesWritten(bytes);
                    }
                }else{
                    break;
                }
            }

            /// @brief 检查服务器状态
            if (tcpSocket->state() == QTcpSocket::UnconnectedState){
                emit messageChanged(tr("服务器已断开"), true);
                break;
            }

            /// @brief 处理线程事件
            eventLoop.processEvents();

            /// @brief 线程睡眠
            threadMutex.lock();
            threadWaitCondition.wait(&threadMutex, debugPage->readWriteParameters().waitForBytesWrittenTime);
            threadMutex.unlock();
        }

        /// @brief 退出前断开与服务器的链接
        if (tcpSocket->state() == QTcpSocket::ClosingState){
            tcpSocket->disconnectFromHost();
            if (tcpSocket->state() == QTcpSocket::ConnectedState){
                if(!tcpSocket->waitForConnected()){
                    emit messageChanged(tr("无法断开与服务器的链接：")+tcpSocket->errorString(), true);
                }
            }
        }
        tcpSocket->close();
        delete tcpSocket;
        emit deviceStateChanged(false);
    }else{
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法打开设备")+tcpSocket->errorString(), false);
    }
}
