/*
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
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

SAKDebuggerDevice::ReadContextVector SAKTcpServerDevice::read()
{
    auto parameters = parametersContext().value<SAKTcpServerParametersContext>();
    QString currentClientHost = parameters.currentClientHost;
    quint16 currentClientPort = parameters.currentClientPort;
    bool all = (currentClientHost == "All Connections");
    ReadContextVector contexts;
    for (const auto &client : mClientList) {
        QString peerHost = client->peerAddress().toString();
        quint16 peerPort = client->peerPort();
        auto bytes = client->readAll();
        if (all || (currentClientHost == peerHost && currentClientPort == peerPort)){
            ReadContext ctx;
            ctx.bytes = bytes;
            ctx.flag = QString("%1:%2").arg(peerHost).arg(peerPort);
            contexts.append(ctx);
        }
    }

    return contexts;
}

SAKDebuggerDevice::WriteContext SAKTcpServerDevice::write(const QByteArray &bytes)
{
    WriteContext context;
    auto parameters = parametersContext().value<SAKTcpServerParametersContext>();
    QString currentClientHost = parameters.currentClientHost;
    quint16 currentClientPort = parameters.currentClientPort;
    bool all = (currentClientHost == "All Connections");
    QString flag;
    for (const auto &client : mClientList) {
        QString peerHost = client->peerAddress().toString();
        quint16 peerPort = client->peerPort();
        if (all || (currentClientHost == peerHost && currentClientPort == peerPort)){
            flag = QString("%1:%2").arg(peerHost).arg(peerPort);
            qint64 ret = client->write(bytes);
            if (ret < 0) {
                qWarning() << QString("Can not write data:(%1)%2")
                              .arg(client->peerAddress().toString(), client->errorString());
            }
        }
    }
    context.bytes = bytes;
    context.flag = all ? "All" : flag;
    return context;
}

void SAKTcpServerDevice::uninitialize()
{
    for (const auto& client : mClientList) {
        client->disconnect();
        client->deleteLater();
    }
    mClientList.clear();
    mTcpServer->close();
    delete mTcpServer;
    mTcpServer = Q_NULLPTR;
}
