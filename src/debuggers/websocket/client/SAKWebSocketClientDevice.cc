/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QWebSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketClientDevice.hh"
#include "SAKWebSocketClientDebugger.hh"
#include "SAKWebSocketClientController.hh"

SAKWebSocketClientDevice::SAKWebSocketClientDevice(QSettings *settings,
                                                   const QString &settingsGroup,
                                                   QWidget *uiParent,
                                                   QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mWebSocket(Q_NULLPTR)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}

bool SAKWebSocketClientDevice::initialize()
{
    QEventLoop *eventLoop = new QEventLoop;
    mWebSocket = new QWebSocket;
    connect(mWebSocket, &QWebSocket::connected, this, [=](){
        QString info = mWebSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mWebSocket->localPort()));
        emit clientInfoChanged(info);
        eventLoop->exit();
    });

    connect(mWebSocket, &QWebSocket::disconnected, this, [=](){
        emit clientInfoChanged(QString());
    });

    connect(mWebSocket, &QWebSocket::binaryFrameReceived,
            this, [=](const QByteArray &message){
        appendMessage(message);
        emit readyRead(SAKDeviceProtectedSignal());
    });

    connect(mWebSocket, &QWebSocket::textMessageReceived, this, [=](QString message){
        appendMessage(message.toUtf8());
        emit readyRead(SAKDeviceProtectedSignal());
    });

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mWebSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
#else
    connect(mWebSocket,
            static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
#endif
            this, [=](QAbstractSocket::SocketError error){
        emit errorOccurred(tr("error code:") + QString::number(error));
        eventLoop->exit(-1);
    });

    auto parameters = parametersContext().value<SAKWSClientParametersContext>();
    mWebSocket->open(parameters.serverAddress);

    int ret = eventLoop->exec();
    if (ret == -1) {
        return false;
    }

    return true;
}

QByteArray SAKWebSocketClientDevice::read()
{
    return takeMessage();
}

QByteArray SAKWebSocketClientDevice::write(const QByteArray &bytes)
{
    if (mWebSocket->state() == QAbstractSocket::ConnectedState){
        qint64 ret = 0;
        auto parameters = parametersContext().value<SAKWSClientParametersContext>();
        if (parameters.sendingType == SAKCommonDataStructure::WebSocketSendingTypeText) {
            ret = mWebSocket->sendTextMessage(QString(bytes));
        } else {
            ret = mWebSocket->sendBinaryMessage(bytes);
        }

        if (ret > 0) {
            return bytes;
        }
    }

    return QByteArray();
}

void SAKWebSocketClientDevice::uninitialize()
{
    mWebSocket->close();
    mWebSocket->deleteLater();
    mWebSocket = Q_NULLPTR;
}

void SAKWebSocketClientDevice::appendMessage(const QByteArray &byteArray)
{
    mByteArrayVectorMutex.lock();
    mByteArrayVector.append(byteArray);
    mByteArrayVectorMutex.unlock();
}

QByteArray SAKWebSocketClientDevice::takeMessage()
{
    QByteArray byteArray;
    mByteArrayVectorMutex.lock();
    if (mByteArrayVector.length()) {
        byteArray = mByteArrayVector.takeFirst();
    }
    mByteArrayVectorMutex.unlock();

    return byteArray;
}
