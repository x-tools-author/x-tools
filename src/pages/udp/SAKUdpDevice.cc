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
#include <QApplication>
#include "SAKUdpDevice.hh"
#include "SAKDebugPage.hh"

SAKUdpDevice::SAKUdpDevice(QString localHost, quint16 localPort,
                           bool enableCustomLocalSetting,
                           QString targetHost, quint16 targetPort,
                           SAKDebugPage *debugPage,
                           QObject *parent)
    :QThread (parent)
    ,localHost (localHost)
    ,localPort (localPort)
    ,enableCustomLocalSetting (enableCustomLocalSetting)
    ,targetHost (targetHost)
    ,targetPort (targetPort)
    ,debugPage (debugPage)
{
    moveToThread(this);
}

SAKUdpDevice::~SAKUdpDevice()
{
    qDebug() << __FUNCTION__;
}

void SAKUdpDevice::run()
{
    udpSocket = new QUdpSocket(this);

    connect(udpSocket, &QUdpSocket::readyRead, this, &SAKUdpDevice::readBytes);
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKUdpDevice::terminate);

    bool bindResult = false;
    if (enableCustomLocalSetting){
        bindResult = udpSocket->bind(QHostAddress(localHost), localPort);
    }else{
        bindResult = udpSocket->bind();
    }

    if (bindResult){
        if (udpSocket->open(QUdpSocket::ReadWrite)){
#ifdef QT_DEBUG
            qDebug() << udpSocket->localAddress().toString() << udpSocket->localPort();
#endif
            emit deviceStatuChanged(true);
            exec();
        }else{
            emit deviceStatuChanged(false);
            emit messageChanged(tr("无法打开设备")+udpSocket->errorString(), false);
        }
    }else{
        emit deviceStatuChanged(false);
        emit messageChanged(tr("无法绑定设备")+udpSocket->errorString(), false);
    }
}

void SAKUdpDevice::readBytes()
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

void SAKUdpDevice::writeBytes(QByteArray data)
{    
    /// @brief 单播
    if (parametersContextInstance().enableUnicast){
        qint64 ret = udpSocket->writeDatagram(data, QHostAddress(targetHost), targetPort);
        udpSocket->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
        if (ret == -1){
            emit messageChanged(tr("发送数据失败：")+udpSocket->errorString(), false);
        }else{
            emit bytesWriten(data);
        }
    }

    /// @brief 组播
    if (parametersContextInstance().enableMulticast){
        for(auto var:parametersContextInstance().multicastInfoList){
            udpSocket->writeDatagram(data, QHostAddress(var.address), var.port);
            emit bytesWriten(data);
        }
    }

    /// @brief 广播
    if (parametersContextInstance().enableBroadcast){
        ParametersContext context = parametersContextInstance();
        udpSocket->writeDatagram(data, QHostAddress::Broadcast, context.broadcastPort);
        emit bytesWriten(data);
#ifdef QT_DEBUG
        qDebug() << __FUNCTION__ << "broadcast" << "->" << "255.255.255.255" << "(" << localHost << ")" << context.broadcastPort;
#endif
    }
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

const SAKUdpDevice::ParametersContext SAKUdpDevice::parametersContextInstance()
{
    parametersContextMutex.lock();
    ParametersContext context = parametersContext;
    parametersContextMutex.unlock();

    return context;
}
