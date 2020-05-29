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
#include <QApplication>
#include "SAKUdpDevice.hh"
#include "SAKUdpDebugPage.hh"

SAKUdpDevice::SAKUdpDevice(SAKUdpDebugPage *debugPage,
                           QObject *parent)
    :SAKDevice(parent)
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
    qDebug() << address;
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
    udpSocket = new QUdpSocket(this);

    bool bindResult = false;
    if (enableCustomLocalSetting){
        bindResult = udpSocket->bind(QHostAddress(localHost), localPort);
    }else{
        bindResult = udpSocket->bind();
    }

    if (bindResult){
        if (udpSocket->open(QUdpSocket::ReadWrite)){
            emit deviceStateChanged(true);
        }else{
            delete udpSocket;
            emit deviceStateChanged(false);
            emit messageChanged(tr("无法打开设备")+udpSocket->errorString(), false);
            return;
        }
    }else{
        delete udpSocket;
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法绑定设备")+udpSocket->errorString(), false);
        return;
    }

    while (true){
        /// @brief 响应外部中断请求
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 读取数据
        readActually();

        /// @brief 发送数据
        while (true){
            QByteArray bytes = takeWaitingForWrittingBytes();
            if (bytes.length()){
                writeActually(bytes);
            }else{
                break;
            }
        }

        /// @brief 线程睡眠
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, debugPage->readWriteParameters().runIntervalTime);
        threadMutex.unlock();
    }
}

void SAKUdpDevice::readActually()
{
    udpSocket->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
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
}

void SAKUdpDevice::writeActually(QByteArray data)
{
    /// @brief 单播
    if (parametersContextInstance().enableUnicast){
        qint64 ret = udpSocket->writeDatagram(data, QHostAddress(targetHost), targetPort);
        udpSocket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
        if (ret == -1){
            emit messageChanged(tr("发送数据失败：")+udpSocket->errorString(), false);
        }else{
            emit bytesWritten(data);
        }
    }

    /// @brief 组播
    if (parametersContextInstance().enableMulticast){
        for(auto var:parametersContextInstance().multicastInfoList){
            udpSocket->writeDatagram(data, QHostAddress(var.address), var.port);
            emit bytesWritten(data);
        }
    }

    /// @brief 广播
    if (parametersContextInstance().enableBroadcast){
        ParametersContext context = parametersContextInstance();
        udpSocket->writeDatagram(data, QHostAddress::Broadcast, context.broadcastPort);
        emit bytesWritten(data);
    }
}

const SAKUdpDevice::ParametersContext SAKUdpDevice::parametersContextInstance()
{
    parametersContextMutex.lock();
    ParametersContext context = parametersContext;
    parametersContextMutex.unlock();

    return context;
}
