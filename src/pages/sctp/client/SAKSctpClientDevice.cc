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

#include "SAKSctpClientDevice.hh"
#include "SAKSctpClientDebugPage.hh"
#include "SAKSctpClientDeviceController.hh"

SAKSctpClientDevice::SAKSctpClientDevice(SAKSctpClientDebugPage *debugPage, QObject *parent)
    :SAKDevice(parent)
    ,debugPage (debugPage)
{

}

void SAKSctpClientDevice::run()
{
    QEventLoop eventLoop;
    SAKSctpClientDeviceController *deviceController = debugPage->controllerInstance();
    localHost = deviceController->localHost();
    localPort = deviceController->localPort();
    serverHost = deviceController->serverHost();
    serverPort = deviceController->serverPort();
    enableCustomLocalSetting = deviceController->enableCustomLocalSetting();

    sctpSocket = new QSctpSocket(Q_NULLPTR);

    bool bindResult = false;
    if (enableCustomLocalSetting){
        bindResult = sctpSocket->bind(QHostAddress(localHost), localPort);
    }else{
        bindResult = sctpSocket->bind();
    }

    if (!bindResult){
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法绑定设备")+sctpSocket->errorString(), false);
        delete sctpSocket;
        return;
    }

    sctpSocket->connectToHost(serverHost, serverPort);
    if (sctpSocket->state() != QTcpSocket::ConnectedState){
        if (!sctpSocket->waitForConnected()){
            emit deviceStateChanged(false);
            emit messageChanged(tr("无法连接到服务器")+sctpSocket->errorString(), false);
            delete sctpSocket;
            return;
        }
    }


    if (sctpSocket->open(QTcpSocket::ReadWrite)){
        emit deviceStateChanged(true);

        while (true) {
            /// @brief 响应中断
            if (isInterruptionRequested()){
                break;
            }

            /// @brief 读取数据
            sctpSocket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
            QByteArray bytes = sctpSocket->readAll();
            if (!bytes.isEmpty()){
                emit bytesRead(bytes);
            }

            /// @brief 发送数据
            while (true){
                QByteArray bytes = takeWaitingForWrittingBytes();
                if (bytes.length()){
                    qint64 ret = sctpSocket->write(bytes);
                    sctpSocket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
                    if (ret == -1){
                        emit messageChanged(tr("发送数据失败：")+sctpSocket->errorString(), false);
                    }else{
                        emit bytesWritten(bytes);
                    }
                }else{
                    break;
                }
            }

            /// @brief 检查服务器状态
            if (sctpSocket->state() == QTcpSocket::UnconnectedState){
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
        if (sctpSocket->state() == QTcpSocket::ClosingState){
            sctpSocket->disconnectFromHost();
            if (sctpSocket->state() == QTcpSocket::ConnectedState){
                if(!sctpSocket->waitForConnected()){
                    emit messageChanged(tr("无法断开与服务器的链接：")+sctpSocket->errorString(), true);
                }
            }
        }
        sctpSocket->close();
        delete sctpSocket;
        emit deviceStateChanged(false);
    }else{
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法打开设备")+sctpSocket->errorString(), false);
    }
}
