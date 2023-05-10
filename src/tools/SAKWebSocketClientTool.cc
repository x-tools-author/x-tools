/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDWebSocketClientTool.hpp"

EDWebSocketClientTool::EDWebSocketClientTool(QObject *parent)
    : EDBaseTool{"ED.WebSocketClientTool", parent}
{

}

bool EDWebSocketClientTool::initialize(QString &errStr)
{
    mWebSocket = new QWebSocket();
    connect(mWebSocket, &QWebSocket::connected, mWebSocket, [=](){
        QString info = mWebSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mWebSocket->localPort()));
        mBindingIpPort = info;
        setToolFlag(info);
        emit bindingIpPortChanged();
    });

    connect(mWebSocket, &QWebSocket::disconnected, mWebSocket, [=](){
        outputMessage(QtInfoMsg, "Connection disconnected:" + mWebSocket->errorString());
    });

    connect(mWebSocket, &QWebSocket::binaryFrameReceived, mWebSocket, [=](const QByteArray &message){
        QString address = mWebSocket->peerAddress().toString();
        quint16 port = mWebSocket->peerPort();
        QString flag = QString("%1:%2").arg(address, QString::number(port));
        emit bytesOutputted(flag, message);
    });

    connect(mWebSocket, &QWebSocket::textMessageReceived, mWebSocket, [=](QString message){
        QString address = mWebSocket->peerAddress().toString();
        quint16 port = mWebSocket->peerPort();
        QString flag = QString("%1:%2").arg(address, QString::number(port));
        emit bytesOutputted(flag, message.toUtf8());
    });

    connect(mWebSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), mWebSocket, [=](QAbstractSocket::SocketError error){
        emit errorOccured(toolFlag(), "Error:" + mWebSocket->errorString());
    });

    QString address = QString("ws://%1:%2").arg(mServerIp, QString::number(mServerPort));
    mWebSocket->open(address);

    return true;
}

void EDWebSocketClientTool::inputBytesHandler(const QByteArray &bytes)
{
    qint64 ret = -1;
    if (mMessageType == 0) {
        ret = mWebSocket->sendBinaryMessage(bytes);
    } else {
        ret = mWebSocket->sendTextMessage(QString::fromUtf8(bytes));
    }

    if (ret == -1) {
        outputMessage(QtWarningMsg, mWebSocket->errorString());
    } else {
        emit bytesInputted(toolFlag(), bytes);
    }
}

void EDWebSocketClientTool::outputBytesHandler()
{

}

void EDWebSocketClientTool::uninitialize()
{
    mWebSocket->close();
    mWebSocket->deleteLater();
    mWebSocket = nullptr;
}
