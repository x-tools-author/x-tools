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
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugPage.hh"
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTcpServerDeviceController.hh"

SAKTcpServerDevice::SAKTcpServerDevice(SAKTcpServerDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
    ,mTcpServer(Q_NULLPTR)
{
#if 0
    mDeviceController = qobject_cast<SAKTcpServerDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKTcpServerDevice::addClient, mDeviceController, &SAKTcpServerDeviceController::addClient);
    connect(this, &SAKTcpServerDevice::removeClient, mDeviceController, &SAKTcpServerDeviceController::removeClient);
#endif
}

bool SAKTcpServerDevice::initializing(QString &errorString)
{
    errorString = tr("Unknown error");
    mDeviceController = qobject_cast<SAKTcpServerDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKTcpServerDevice::addClient, mDeviceController, &SAKTcpServerDeviceController::addClient);
    connect(this, &SAKTcpServerDevice::removeClient, mDeviceController, &SAKTcpServerDeviceController::removeClient);

    return true;
}

bool SAKTcpServerDevice::open(QString &errorString)
{
    auto parameters = mDeviceController->parameters().value<SAKTcpServerDeviceController::TcpServerParameters>();
    QString serverHost = parameters.serverHost;
    quint16 serverPort = parameters.serverPort;

    mTcpServer = new QTcpServer;
    if (!mTcpServer->listen(QHostAddress(serverHost), serverPort)){
        errorString = tr("Listen failed:") + mTcpServer->errorString();
        return false;
    }

    return true;
}

QByteArray SAKTcpServerDevice::read()
{
    for (auto &var : mClientList){
         QByteArray bytes = var->readAll();
         auto parameters = mDeviceController->parameters().value<SAKTcpServerDeviceController::TcpServerParameters>();
         QString currentClientHost = parameters.currentClientHost;
         QString peerHost = var->peerAddress().toString();
         quint16 currentClientPort = parameters.currentClientPort;
         quint16 peerPort = var->peerPort();

         if (bytes.length()){
             if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
                 return bytes;
             }
         }
    }

    return QByteArray();
}

QByteArray SAKTcpServerDevice::write(QByteArray bytes)
{
    for (auto &var : mClientList){
        auto parameters = mDeviceController->parameters().value<SAKTcpServerDeviceController::TcpServerParameters>();
        QString currentClientHost = parameters.currentClientHost;
        QString peerHost = var->peerAddress().toString();
        quint16 currentClientPort = parameters.currentClientPort;
        quint16 peerPort = var->peerPort();
        if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
            qint64 ret = var->write(bytes);
            if (ret > 0){
                return bytes;
            }else{
                qDebug() << __FUNCTION__ << QString("Can not write data:(%1)%2").arg(var->peerAddress().toString().arg(var->error()));
            }
        }
    }

    return QByteArray();
}

bool SAKTcpServerDevice::checkSomething(QString &errorString)
{
    // Handling new connection
    while (mTcpServer->hasPendingConnections()){
        QTcpSocket *socket = mTcpServer->nextPendingConnection();
        if (socket){
            emit addClient(socket->peerAddress().toString(), socket->peerPort(), socket);
            mClientList.append(socket);
        }
    }

    // Remove clients which is offline
    for (auto &var : mClientList){
        QList<QTcpSocket*> offLineClientList;
        if (var->state() != QTcpSocket::ConnectedState){
            offLineClientList.append(var);
        }

        for (auto &var : offLineClientList){
            emit removeClient(var);
            mClientList.removeOne(var);
        }
    }

    errorString = tr("Unknown error");
    return true;
}

void SAKTcpServerDevice::close()
{
    mTcpServer->close();
}

void SAKTcpServerDevice::free()
{
    delete mTcpServer;
    mTcpServer = Q_NULLPTR;
}
