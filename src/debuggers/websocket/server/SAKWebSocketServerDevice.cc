/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugger.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketServerDevice.hh"
#include "SAKWebSocketServerDebugger.hh"
#include "SAKWebSocketServerController.hh"

SAKWebSocketServerDevice::SAKWebSocketServerDevice(QSettings *settings,
                                                   const QString &settingsGroup,
                                                   QWidget *uiParent,
                                                   QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mWebSocketServer(Q_NULLPTR)
{

}

bool SAKWebSocketServerDevice::initialize()
{
    auto parameters = parametersContext().value<SAKWSServerParametersContext>();
    QString serverName = QString("%1:%2").arg(parameters.serverHost).arg(parameters.serverPort);
    mWebSocketServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    if (!mWebSocketServer->listen(QHostAddress(parameters.serverHost), parameters.serverPort)) {
        QString errorString = tr("Listen failed:") + mWebSocketServer->errorString();
        emit errorOccurred(errorString);
        return false;
    }

    connect(mWebSocketServer, &QWebSocketServer::newConnection, this, [=](){
        while (mWebSocketServer->hasPendingConnections()){
            QWebSocket *socket = mWebSocketServer->nextPendingConnection();
            if (socket){
                mClientList.append(socket);
                emit addClient(socket->peerAddress().toString(), socket->peerPort(), socket);

                connect(socket, &QWebSocket::textMessageReceived, socket, [=](const QString message){
                    readBytesActually(socket, message.toUtf8());
                });

                connect(socket, &QWebSocket::binaryMessageReceived, socket, [=](const QByteArray message){
                    readBytesActually(socket, message);
                });

                connect(socket, &QWebSocket::disconnected, socket, [&](){
                    emit removeClient(socket);
                    mClientList.removeOne(socket);
                });
            }
        }
    });

    return true;
}

SAKDebuggerDevice::ReadContextVector SAKWebSocketServerDevice::read()
{
    return ReadContextVector();
}

SAKDebuggerDevice::WriteContext SAKWebSocketServerDevice::write(const QByteArray &bytes)
{
    WriteContext context;
    auto parameters = parametersContext().value<SAKWSServerParametersContext>();
    QString currentClientHost = parameters.currentClientHost;
    quint16 currentClientPort = parameters.currentClientPort;
    bool all = (currentClientHost == "All Connections");
    QString flag;
    for (auto &var : mClientList){
        QString peerHost = var->peerAddress().toString();
        quint16 peerPort = var->peerPort();

        if (all || (currentClientHost == peerHost && currentClientPort == peerPort)){
            flag = QString("%1:%2").arg(peerHost).arg(peerPort);
            qint64 ret = 0;
            if (parameters.sendingType == SAKCommonDataStructure::WebSocketSendingTypeText){
                ret = var->sendTextMessage(QString(bytes));
            }else{
                ret = var->sendBinaryMessage(bytes);
            }
        }
    }
    context.bytes = bytes;
    context.flag = all ? "All" : flag;
    return context;
}

void SAKWebSocketServerDevice::uninitialize()
{
    emit clearClient();
    mWebSocketServer->close();
    delete mWebSocketServer;
    mWebSocketServer = Q_NULLPTR;
}

void SAKWebSocketServerDevice::readBytesActually(QWebSocket *socket, QByteArray bytes)
{
    auto parameters = parametersContext().value<SAKWSServerParametersContext>();
    QString currentClientHost = parameters.currentClientHost;
    QString peerHost = socket->peerAddress().toString();
    quint16 currentClientPort = parameters.currentClientPort;
    quint16 peerPort = socket->peerPort();

    bool all = (currentClientHost == "All Connections");
    if (bytes.length()){
        if (all || (currentClientHost == peerHost && currentClientPort == peerPort)){
            emit bytesRead(bytes, peerHost + ":"
                           + QString::number(peerPort));
        }
    }
}
