/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKUdpDevice.hh"
#include "SAKUdpDebugPage.hh"
#include "SAKUdpDeviceController.hh"

SAKUdpDevice::SAKUdpDevice(SAKUdpDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,enableCustomLocalSetting(false)
    ,debugPage (debugPage)
{
    parametersContext.enableUnicast = true;
    parametersContext.enableMulticast = false;
    parametersContext.enableBroadcast = false;
}

SAKUdpDevice::~SAKUdpDevice()
{

}

void SAKUdpDevice::setUnicastEnable(bool enable)
{
    parametersContextMutex.lock();
    parametersContext.enableUnicast = enable;
    parametersContextMutex.unlock();
}

void SAKUdpDevice::setBroadcastEnable(bool enable)
{
    parametersContextMutex.lock();
    parametersContext.enableBroadcast = enable;
    parametersContextMutex.unlock();
}

void SAKUdpDevice::setBroadcastPort(quint16 port)
{
    parametersContextMutex.lock();
    parametersContext.broadcastPort = port;
    parametersContextMutex.unlock();
}

void SAKUdpDevice::addMulticastInfo(QString address, quint16 port)
{
    parametersContextMutex.lock();
    ParametersContext::MulticastInfo info{address, port};
    parametersContext.multicastInfoList.append(info);
    udpSocket->joinMulticastGroup(QHostAddress(address));
    parametersContextMutex.unlock();
}

void SAKUdpDevice::removeMulticastInfo(QString address, quint16 port)
{
    parametersContextMutex.lock();
    for(int i = 0; 0 < parametersContext.multicastInfoList.length(); i++){
        ParametersContext::MulticastInfo info = parametersContext.multicastInfoList.at(i);
        if ((address == info.address) && (port == info.port)){
            udpSocket->leaveMulticastGroup(QHostAddress(address));
            parametersContext.multicastInfoList.removeAt(i);
            udpSocket->leaveMulticastGroup(QHostAddress(address));
        }
    }
    parametersContextMutex.unlock();
}

void SAKUdpDevice::setMulticastEnable(bool enable)
{
    parametersContextMutex.lock();
    parametersContext.enableMulticast = enable;
    parametersContextMutex.unlock();
}

void SAKUdpDevice::run()
{
    QEventLoop eventLoop;
    SAKUdpDeviceController *deviceController = debugPage->controllerInstance();
    enableCustomLocalSetting = deviceController->enableCustomLocalSetting();
    localHost = deviceController->localHost();
    localPort = deviceController->localPort();
    targetHost = deviceController->targetHost();
    targetPort = deviceController->targetPort();

    udpSocket = new QUdpSocket;
    bool bindResult = false;
    if (enableCustomLocalSetting){
        if (localHost.compare(SAK_HOST_ADDRESS_ANY) == 0){
            bindResult = udpSocket->bind(QHostAddress::Any, localPort);
        }else{
            bindResult = udpSocket->bind(QHostAddress(localHost), localPort);
        }
    }else{
        bindResult = udpSocket->bind();
    }

    if (!bindResult){
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法绑定设备")+udpSocket->errorString(), false);
        delete udpSocket;
        return;
    }

    if (udpSocket->open(QUdpSocket::ReadWrite)){
        emit deviceStateChanged(true);
        while (true){
            /// @brief 响应外部中断请求
            if (isInterruptionRequested()){
                break;
            }

            /// @brief 读取数据
//            udpSocket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
            while (udpSocket->hasPendingDatagrams()) {
                QByteArray data;
                data.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
                qint64 ret = udpSocket->readDatagram(data.data(), data.length());
                if (ret == -1){
                    emit messageChanged(tr("读取数据失败：")+udpSocket->errorString(), false);
                }else{
                    emit bytesRead(data);
                }
            }

            /// @brief 发送数据
            while (true){
                QByteArray bytes = takeWaitingForWrittingBytes();
                if (bytes.length()){
                    /// @brief 单播
                    if (parametersContextInstance().enableUnicast){
                        qint64 ret = udpSocket->writeDatagram(bytes, QHostAddress(targetHost), targetPort);
//                        udpSocket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
                        if (ret == -1){
                            emit messageChanged(tr("发送数据失败：")+udpSocket->errorString(), false);
                        }else{
                            emit bytesWritten(bytes);
                        }
                    }

                    /// @brief 组播
                    if (parametersContextInstance().enableMulticast){
                        for(auto var:parametersContextInstance().multicastInfoList){
                            udpSocket->writeDatagram(bytes, QHostAddress(var.address), var.port);
                            emit bytesWritten(bytes);
                        }
                    }

                    /// @brief 广播
                    if (parametersContextInstance().enableBroadcast){
                        ParametersContext context = parametersContextInstance();
                        udpSocket->writeDatagram(bytes, QHostAddress::Broadcast, context.broadcastPort);
                        emit bytesWritten(bytes);
                    }
                }else{
                    break;
                }
            }

            /// @brief 线程事件处理
            eventLoop.processEvents();

            /// @brief 线程睡眠
            mThreadMutex.lock();
            mThreadWaitCondition.wait(&mThreadMutex, SAK_DEVICE_THREAD_SLEEP_INTERVAL);
            mThreadMutex.unlock();
        }

        udpSocket->close();
        delete udpSocket;
        udpSocket = Q_NULLPTR;
        emit deviceStateChanged(false);
    }else{
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法打开设备")+udpSocket->errorString(), false);
        delete udpSocket;
    }
}

const SAKUdpDevice::ParametersContext SAKUdpDevice::parametersContextInstance()
{
    parametersContextMutex.lock();
    ParametersContext context = parametersContext;
    parametersContextMutex.unlock();

    return context;
}
