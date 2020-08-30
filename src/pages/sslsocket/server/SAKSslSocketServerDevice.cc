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
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugPage.hh"
#include "SAKSslSocketTcpServer.hh"
#include "SAKSslSocketServerDevice.hh"
#include "SAKSslSocketServerDebugPage.hh"
#include "SAKSslSocketServerDeviceController.hh"

SAKSslSocketServerDevice::SAKSslSocketServerDevice(SAKSslSocketServerDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,mDebugPage(debugPage)
    ,mTcpServer(Q_NULLPTR)
{
#if 0
    mDeviceController = qobject_cast<SAKSslSocketServerDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKSslSocketServerDevice::addClient, mDeviceController, &SAKSslSocketServerDeviceController::addClient);
    connect(this, &SAKSslSocketServerDevice::removeClient, mDeviceController, &SAKSslSocketServerDeviceController::removeClient);
#endif
}

bool SAKSslSocketServerDevice::initializing(QString &errorString)
{
    errorString = tr("Unknow error");
    mDeviceController = qobject_cast<SAKSslSocketServerDeviceController*>(mDebugPage->deviceController());
    connect(this, &SAKSslSocketServerDevice::addClient, mDeviceController, &SAKSslSocketServerDeviceController::addClient);
    connect(this, &SAKSslSocketServerDevice::removeClient, mDeviceController, &SAKSslSocketServerDeviceController::removeClient);

    return true;
}

bool SAKSslSocketServerDevice::open(QString &errorString)
{
    auto parameters = mDeviceController->parameters().value<SAKSslSocketServerDeviceController::SslSocketServerParameters>();
    QString serverHost = parameters.serverHost;
    quint16 serverPort = parameters.serverPort;

    mTcpServer = new SAKSslSocketTcpServer;
    if (!mTcpServer->listen(QHostAddress(serverHost), serverPort)){
        errorString = tr("Listen failed:") + mTcpServer->errorString();
        return false;
    }

    return true;
}

QByteArray SAKSslSocketServerDevice::read()
{
    for (auto var : mTcpServer->clients()){
         QByteArray bytes = var->readAll();
         auto parameters = mDeviceController->parameters().value<SAKSslSocketServerDeviceController::SslSocketServerParameters>();
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

QByteArray SAKSslSocketServerDevice::write(QByteArray bytes)
{
    for (auto var : mTcpServer->clients()){
        auto parameters = mDeviceController->parameters().value<SAKSslSocketServerDeviceController::SslSocketServerParameters>();
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

bool SAKSslSocketServerDevice::checkSomething(QString &errorString)
{
    // Remove clients which is offline
    for (auto var : mTcpServer->clients()){
        QList<QSslSocket*> offLineClientList;
        if (var->state() != QTcpSocket::ConnectedState){
            offLineClientList.append(var);
        }

        for (auto var : offLineClientList){
            emit removeClient(var);
            mTcpServer->removeClient(var);
        }
    }

    errorString = tr("Unknow error");
    return true;
}

void SAKSslSocketServerDevice::close()
{
    mTcpServer->close();
}

void SAKSslSocketServerDevice::free()
{
    delete mTcpServer;
    mTcpServer = Q_NULLPTR;
}
