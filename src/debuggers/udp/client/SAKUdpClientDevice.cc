/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpClientDeviceController.hh"

SAKUdpClientDevice::SAKUdpClientDevice(SAKUdpClientDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
{
    mParametersContext.enableUnicast = true;
    mParametersContext.enableMulticast = false;
    mParametersContext.enableBroadcast = false;
    qRegisterMetaType<SAKUdpClientDevice::UdpSocketParameters::MulticastInfo>("SAKUdpClientDevice::UdpSocketParameters::MulticastInfo");
}

SAKUdpClientDevice::~SAKUdpClientDevice()
{

}

void SAKUdpClientDevice::setUnicastEnable(bool enable)
{
    mParametersContextMutex.lock();
    mParametersContext.enableUnicast = enable;
    mParametersContextMutex.unlock();
}

void SAKUdpClientDevice::setBroadcastEnable(bool enable)
{
    mParametersContextMutex.lock();
    mParametersContext.enableBroadcast = enable;
    mParametersContextMutex.unlock();
}

void SAKUdpClientDevice::setBroadcastPort(quint16 port)
{
    mParametersContextMutex.lock();
    mParametersContext.broadcastPort = port;
    mParametersContextMutex.unlock();
}

bool SAKUdpClientDevice::joinMulticastGroup(QString address, quint16 port, QString &errorString)
{
    mParametersContextMutex.lock();
     UdpSocketParameters::MulticastInfo info{address, port};
    mParametersContext.multicastInfoList.append(info);
    bool ret = mUdpSocket->joinMulticastGroup(QHostAddress(address));
    if (ret){
        errorString = QString("Unknown error");
    }else{
        errorString = mUdpSocket->errorString();
        qDebug() << __FUNCTION__ << address << port << errorString;
    }
    mParametersContextMutex.unlock();

    return ret;
}

void SAKUdpClientDevice::removeMulticastInfo(QString address, quint16 port)
{
    mParametersContextMutex.lock();
    for(int i = 0; 0 < mParametersContext.multicastInfoList.length(); i++){
         UdpSocketParameters::MulticastInfo info = mParametersContext.multicastInfoList.at(i);
        if ((address == info.address) && (port == info.port)){
            mUdpSocket->leaveMulticastGroup(QHostAddress(address));
            mParametersContext.multicastInfoList.removeAt(i);
            mUdpSocket->leaveMulticastGroup(QHostAddress(address));
        }
    }
    mParametersContextMutex.unlock();
}

void SAKUdpClientDevice::setMulticastEnable(bool enable)
{
    mParametersContextMutex.lock();
    mParametersContext.enableMulticast = enable;
    mParametersContextMutex.unlock();
}

bool SAKUdpClientDevice::initializing(QString &errorString)
{
    mDeviceController = qobject_cast<SAKUdpClientDeviceController*>(mDebugPage->deviceController());
    auto parameters = mDeviceController->parameters().value<SAKUdpClientDeviceController::UdpClientParameters>();
    connect(this, &SAKUdpClientDevice::clientInfoChanged, mDeviceController, &SAKUdpClientDeviceController::setClientInfo);
    bool specifyClientAddressAndPort = parameters.specifyClientAddressAndPort;
    QString localHost = parameters.localHost;
    quint16 localPort = parameters.localPort;

    mUdpSocket = new QUdpSocket;
    bool bindResult = false;
    if (specifyClientAddressAndPort){
        if (localHost.compare(SAK_HOST_ADDRESS_ANY) == 0){
            bindResult = mUdpSocket->bind(QHostAddress::Any, localPort, QUdpSocket::ShareAddress);
        }else{
            bindResult = mUdpSocket->bind(QHostAddress(localHost), localPort, QUdpSocket::ShareAddress);
        }
    }else{
        bindResult = mUdpSocket->bind();
    }

    if (bindResult){
        QString info = mUdpSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mUdpSocket->localPort()));
        emit clientInfoChanged(info);
    }else{
        errorString = tr("Binding failed:") + mUdpSocket->errorString();
        return false;
    }

    return true;
}

bool SAKUdpClientDevice::open(QString &errorString)
{
    if (!mUdpSocket->open(QUdpSocket::ReadWrite)){
        errorString = tr("Open device failed:") + mUdpSocket->errorString();
        return false;
    }

    errorString = tr("Unknown error");
    return true;
}

QByteArray SAKUdpClientDevice::read()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(static_cast<int>(mUdpSocket->pendingDatagramSize()));
        qint64 ret = mUdpSocket->readDatagram(data.data(), data.length());
        if (ret > 0){
            emit bytesRead(data);
        }
    }

    return QByteArray();
}

QByteArray SAKUdpClientDevice::write(QByteArray bytes)
{
    // Unicast
    auto parameters = mDeviceController->parameters().value<SAKUdpClientDeviceController::UdpClientParameters>();
    if (udpSocketParameters().enableUnicast){
        qint64 ret = mUdpSocket->writeDatagram(bytes, QHostAddress(parameters.targetHost), parameters.targetPort);
        if (ret > 0){
            emit bytesWritten(bytes);
        }
    }

    // Multicast
    if (udpSocketParameters().enableMulticast){
         UdpSocketParameters parameters = udpSocketParameters();
        for(auto &var : parameters.multicastInfoList){
            qint16 ret = mUdpSocket->writeDatagram(bytes, QHostAddress(var.address), var.port);
            if (ret > 0){
                emit bytesWritten(bytes);
            }
        }
    }

    // Broadcast
    if (udpSocketParameters().enableBroadcast){
         UdpSocketParameters parameters = udpSocketParameters();
        qint16 ret = mUdpSocket->writeDatagram(bytes, QHostAddress::Broadcast, parameters.broadcastPort);
        if (ret > 0){
            emit bytesWritten(bytes);
        }
    }

    return QByteArray();
}

bool SAKUdpClientDevice::checkSomething(QString &errorString)
{
    errorString = tr("Unknown error");
    return true;
}

void SAKUdpClientDevice::close()
{
    emit clientInfoChanged(QString());
    mUdpSocket->close();
}

void SAKUdpClientDevice::free()
{
    delete mUdpSocket;
    mUdpSocket = Q_NULLPTR;
}

const SAKUdpClientDevice:: UdpSocketParameters SAKUdpClientDevice::udpSocketParameters()
{
    mParametersContextMutex.lock();
     UdpSocketParameters context = mParametersContext;
    mParametersContextMutex.unlock();

    return context;
}
