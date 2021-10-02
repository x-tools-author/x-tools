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

#include "SAKDebugger.hh"
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugger.hh"
#include "SAKTcpServerController.hh"
#include "SAKCommonDataStructure.hh"

SAKTcpServerDevice::SAKTcpServerDevice(QSettings *settings,
                                       const QString &settingsGroup,
                                       QWidget *uiParent,
                                       QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mTcpServer(Q_NULLPTR)
{

}

bool SAKTcpServerDevice::initialize()
{
    auto parameters = parametersContext().value<SAKTcpServerParametersContext>();
    QString serverHost = parameters.serverHost;
    quint16 serverPort = parameters.serverPort;

    mTcpServer = new QTcpServer;
    if (mTcpServer->listen(QHostAddress(serverHost), serverPort)){
        connect(mTcpServer, &QTcpServer::newConnection, this, [=](){
            QTcpSocket *socket = mTcpServer->nextPendingConnection();
            if (socket){
                emit addClient(socket->peerAddress().toString(), socket->peerPort(), socket);
                mClientList.append(socket);
            }

            connect(socket, &QTcpSocket::disconnected, this, [=](){
                emit removeClient(socket);
                mClientList.removeOne(socket);
            });

            connect(socket, &QTcpSocket::readyRead, this, [=](){
                emit readyRead(SAKDeviceProtectedSignal());
            });
        });
    } else {
        QString errorString = tr("Listen failed:") + mTcpServer->errorString();
        emit errorOccurred(errorString);
        return false;
    }

    return true;
}

QByteArray SAKTcpServerDevice::read()
{
    for (int i = 0; i < mClientList.length(); i++) {
        auto client = mClientList.at(i);
         QByteArray bytes = client->readAll();
         auto parameters = parametersContext().value<SAKTcpServerParametersContext>();
         QString currentClientHost = parameters.currentClientHost;
         QString peerHost = client->peerAddress().toString();
         quint16 currentClientPort = parameters.currentClientPort;
         quint16 peerPort = client->peerPort();

         if (bytes.length()){
             if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
                 return bytes;
             }
         }
    }

    return QByteArray();
}

QByteArray SAKTcpServerDevice::write(const QByteArray &bytes)
{
    for (int i = 0; i < mClientList.length(); i++) {
        auto client = mClientList.at(i);
        auto parameters = parametersContext().value<SAKTcpServerParametersContext>();
        QString currentClientHost = parameters.currentClientHost;
        QString peerHost = client->peerAddress().toString();
        quint16 currentClientPort = parameters.currentClientPort;
        quint16 peerPort = client->peerPort();
        if ((currentClientHost == peerHost) && (currentClientPort == peerPort)){
            qint64 ret = client->write(bytes);
            if (ret > 0) {
                return bytes;
            } else {
                qWarning() << QString("Can not write data:(%1)%2")
                              .arg(client->peerAddress().toString(), client->errorString());
            }
        }
    }

    return QByteArray();
}

void SAKTcpServerDevice::uninitialize()
{
    mTcpServer->close();
    delete mTcpServer;
    mTcpServer = Q_NULLPTR;
}
