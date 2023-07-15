/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKModbusClient.hh"

SAKModbusClient::SAKModbusClient(const char *lc, QObject *parent)
    : SAKModbusDevice(lc, parent)
{
    mClientParameters.numberOfRetries = 3;
    mClientParameters.timeout = 1000;
}

void SAKModbusClient::sendRawRequest(const QModbusRequest &request,
                                     int serverAddress)
{
    emit invokeSendRawRequest(request, serverAddress);
}

void SAKModbusClient::sendReadRequest(const QModbusDataUnit &read,
                                      int serverAddress)
{
    emit invokeSendReadRequest(read, serverAddress);
}

void SAKModbusClient::sendReadWriteRequest(const QModbusDataUnit &read,
                          const QModbusDataUnit &write, int serverAddress)
{
    emit invokeSendReadWriteRequest(read, write, serverAddress);
}

void SAKModbusClient::sendWriteRequest(const QModbusDataUnit &write,
                                       int serverAddress)
{
    emit invokeSendWriteRequest(write, serverAddress);
}

void SAKModbusClient::run()
{
    mConnectParametersMutex.lock();
    ConnectionParameters ctx = mConnectParameters;
    mConnectParametersMutex.unlock();
    QString clientParameters;
    clientParameters += "portName:" + ctx.serialport.portName + "; ";
    clientParameters += "baudRate:" + QString::number(ctx.serialport.baudRate) + "; ";
    clientParameters += "stopBits:" + QString::number(ctx.serialport.stopBits) + "; ";
    clientParameters += "dataBits:" + QString::number(ctx.serialport.dataBits) + "; ";
    clientParameters += "parity:" +  QString::number(ctx.serialport.parity) + "; ";
    clientParameters += "ip:" + ctx.network.ip+ "; ";
    clientParameters += "port:" +  QString::number(ctx.network.port);
    outputLog(clientParameters, QtInfoMsg);

    mClient = createClient();
    if (!mClient) {
        outputLog("Invalid client", QtWarningMsg);
        return;
    }

    connect(mClient, &QModbusClient::errorOccurred, this, [=](){
        const QString errStr = mClient->errorString();
        outputLog(errStr, QtWarningMsg);
        emit errorOccurred(errStr);
        exit();
    });

    connect(mClient, &QModbusClient::stateChanged, this, [=](){
        int state = int(mClient->state());
        outputLog("Client state changed: " + QString::number(state),
                  QtWarningMsg);
        emit stateChanged(state);
    });

    connect(this, &SAKModbusClient::invokeSendRawRequest,
            mClient, [=](const QModbusRequest &request, int serverAddress){
        this->onInvokeSendRawRequest(request, serverAddress);
    });
    connect(this, &SAKModbusClient::invokeSendReadRequest,
            mClient, [=](const QModbusDataUnit &read, int serverAddress){
        this->onInvokeSendReadRequest(read, serverAddress);
    });
    connect(this, &SAKModbusClient::invokeSendReadWriteRequest,
            mClient, [=](const QModbusDataUnit &read,
                         const QModbusDataUnit &write, int serverAddress){
        this->onInvokeSendReadWriteRequest(read, write, serverAddress);
    });
    connect(this, &SAKModbusClient::invokeSendReadRequest,
            mClient, [=](const QModbusDataUnit &write, int serverAddress){
        this->onInvokeSendWriteRequest(write, serverAddress);
    });

    mClient->connectDevice();
    exec();
    mClient->disconnectDevice();
    mClient->deleteLater();
    mClient = Q_NULLPTR;
}

void SAKModbusClient::onInvokeSendRawRequest(const QModbusRequest &request,
                                             int serverAddress)
{
    if (!mClient) {
        return;
    }

    QModbusReply *reply = mClient->sendRawRequest(request, serverAddress);
    if (!reply) {
        return;
    }

    connect(reply, &QModbusReply::finished, this, [=](){
        if (reply->error() != QModbusDevice::NoError) {
            outputLog(reply->errorString(), QtWarningMsg);
            return;
        }

        reply->rawResult().data();
    });
}

void SAKModbusClient::onInvokeSendReadRequest(const QModbusDataUnit &read,
                                              int serverAddress)
{
    if (!mClient) {
        return;
    }

    QModbusReply *reply = mClient->sendReadRequest(read, serverAddress);
    if (!reply) {
        return;
    }

    connect(reply, &QModbusReply::finished, this, [=](){
        if (reply->error() != QModbusDevice::NoError) {
            outputLog(reply->errorString(), QtWarningMsg);
            return;
        }
    });
}

void SAKModbusClient::onInvokeSendReadWriteRequest(const QModbusDataUnit &read,
                                                   const QModbusDataUnit &write,
                                                   int serverAddress)
{
    if (!mClient) {
        return;
    }

    QModbusReply *reply = mClient->sendReadWriteRequest(read, write,
                                                        serverAddress);
    if (!reply) {
        return;
    }

    connect(reply, &QModbusReply::finished, this, [=](){
        if (reply->error() != QModbusDevice::NoError) {
            outputLog(reply->errorString(), QtWarningMsg);
            return;
        }
    });
}

void SAKModbusClient::onInvokeSendWriteRequest(const QModbusDataUnit &write,
                                               int serverAddress)
{
    if (!mClient) {
        return;
    }

    QModbusReply *reply = mClient->sendWriteRequest(write, serverAddress);
    if (!reply) {
        return;
    }

    connect(reply, &QModbusReply::finished, this, [=](){
        if (reply->error() != QModbusDevice::NoError) {
            outputLog(reply->errorString(), QtWarningMsg);
            return;
        }
    });
}

int SAKModbusClient::numberOfRetries()
{
    mClientParametersMutex.lock();
    int numberOfRetries = mClientParameters.numberOfRetries;
    mClientParametersMutex.unlock();

    return numberOfRetries;
}

void SAKModbusClient::setNumberOfRetries(int numberOfRetries)
{
    mClientParametersMutex.lock();
    mClientParameters.numberOfRetries = numberOfRetries;
    mClientParametersMutex.unlock();
}

int SAKModbusClient::timeout()
{
    mClientParametersMutex.lock();
    int timeout = mClientParameters.timeout;
    mClientParametersMutex.unlock();

    return timeout;
}

void SAKModbusClient::setTimeout(int timeout)
{
    mClientParametersMutex.lock();
    mClientParameters.timeout = timeout;
    mClientParametersMutex.unlock();
}
