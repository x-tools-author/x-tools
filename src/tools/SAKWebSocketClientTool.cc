/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKWebSocketClientTool.hh"

SAKWebSocketClientTool::SAKWebSocketClientTool(QObject *parent)
    : SAKSocketClientTool{"SAK.WebSocketClientTool", parent}
{

}

bool SAKWebSocketClientTool::initialize()
{
    mWebSocket = new QWebSocket();
    connect(mWebSocket, &QWebSocket::connected, mWebSocket, [=](){
        QString info = mWebSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mWebSocket->localPort()));
        mBindingIpPort = info;
        emit bindingIpPortChanged();
    });

    connect(mWebSocket, &QWebSocket::disconnected, mWebSocket, [=](){
        outputMessage(QtInfoMsg, QString("Connection(%1) disconnected:%2")
                                     .arg(mBindingIpPort,
                                          mWebSocket->errorString()));
    });

    connect(mWebSocket, &QWebSocket::binaryFrameReceived,
            mWebSocket, [=](const QByteArray &message){
        QString ip = mWebSocket->peerAddress().toString();
        quint16 port = mWebSocket->peerPort();
        QString ipport = QString("%1:%2").arg(ip, QString::number(port));
        QString hex = QString::fromLatin1(message.toHex(' '));
        outputMessage(QtInfoMsg,
                      QString("%1<-%2:%3").arg(mBindingIpPort, ipport, hex));
        emit bytesOutputted(message, QVariant());
    });

    connect(mWebSocket, &QWebSocket::textMessageReceived,
            mWebSocket, [=](QString message){
        QString ip = mWebSocket->peerAddress().toString();
        quint16 port = mWebSocket->peerPort();
        QString ipport = QString("%1:%2").arg(ip, QString::number(port));
        outputMessage(QtInfoMsg, QString("%1<-%2:%3")
                                     .arg(mBindingIpPort, ipport, message));
        emit bytesOutputted(message.toUtf8(), QVariant());
    });

    connect(mWebSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            mWebSocket, [=](QAbstractSocket::SocketError error){
        Q_UNUSED(error);
        outputMessage(QtInfoMsg, QString("Error occured:")
                                     .arg(mWebSocket->errorString()));
        emit errorOccured("Error:" + mWebSocket->errorString());
    });

    QString address = QString("ws://%1:%2").arg(mServerIp,
                                                QString::number(mServerPort));
    mWebSocket->open(address);

    return true;
}

void SAKWebSocketClientTool::writeBytes(const QByteArray &bytes,
                                        const QVariant &context)
{
    Q_UNUSED(context);
    qint64 ret = -1;
    QString hex;
    if (mMessageType == 0) {
        hex = QString::fromLatin1(bytes.toHex(' '));
        ret = mWebSocket->sendBinaryMessage(bytes);
    } else {
        hex = QString::fromUtf8(bytes);
        ret = mWebSocket->sendTextMessage(QString::fromUtf8(bytes));
    }

    if (ret == -1) {
        outputMessage(QtWarningMsg, mWebSocket->errorString());
    } else {
        QString ip = mWebSocket->peerAddress().toString();
        quint16 port = mWebSocket->peerPort();
        QString ipport = QString("%1:%2").arg(ip, QString::number(port));
        outputMessage(QtInfoMsg, QString("%1->%2:%3")
                                     .arg(mBindingIpPort, ipport, hex));
        emit bytesInputted(bytes, QVariant());
    }
}

void SAKWebSocketClientTool::uninitialize()
{
    mWebSocket->close();
    mWebSocket->deleteLater();
    mWebSocket = nullptr;
}
