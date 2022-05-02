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
#include <QTimer>
#include <QHostAddress>
#include <QApplication>

#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugger.hh"
#include "SAKTcpClientController.hh"
#include "SAKCommonDataStructure.hh"

SAKTcpClientDevice::SAKTcpClientDevice(QSettings *settings,
                                       const QString &settingsGroup,
                                       QWidget *uiParent,
                                       QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
{
    // Reconnection
    connect(this, &SAKTcpClientDevice::finished, this, [&](){
        auto parameters = parametersContext()
                .value<SAKTcpClientParametersContext>();
        if (parameters.allowAutomaticConnection){
            QTimer *timer = new QTimer(this);
            timer->setInterval(2000);
            timer->setSingleShot(true);
            connect(timer, &QTimer::timeout, this, [=](){
                timer->deleteLater();
                start();
            });
            timer->start();
        }
    });
}

bool SAKTcpClientDevice::initialize()
{
    auto parameters = parametersContext()
            .value<SAKTcpClientParametersContext>();
    mLocalHost = parameters.localHost;
    mLocalPort = parameters.localPort;
    mServerHost = parameters.serverHost;
    mServerPort = parameters.serverPort;
    mSpecifyClientAddressAndPort = parameters.specifyClientAddressAndPort;

    mTcpSocket = new QTcpSocket;
    bool bindResult = false;
    if (mSpecifyClientAddressAndPort){
        bindResult = mTcpSocket->bind(QHostAddress(mLocalHost), mLocalPort);
    }else{
        bindResult = mTcpSocket->bind();
    }

    if (!bindResult){
        QString errorString = tr("Binding failed:") + mTcpSocket->errorString();
        emit errorOccurred(errorString);
        return false;
    }else{
        QString info = mTcpSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mTcpSocket->localPort()));
        emit serverInfoChanged(info);
    }

    mTcpSocket->connectToHost(mServerHost, mServerPort);
    if (mTcpSocket->state() != QTcpSocket::ConnectedState){
        if (!mTcpSocket->waitForConnected()){
            QString errorString = tr("Connect to server failed:")
                    + mTcpSocket->errorString();
            emit errorOccurred(errorString);
            return false;
        }
    }

    if (mTcpSocket->open(QTcpSocket::ReadWrite)){
        connect(mTcpSocket, &QTcpSocket::readyRead, this, [=](){
            emit readyRead(SAKDeviceProtectedSignal());
        });
    }else{
        QString errorString = tr("Can not open device:") + mTcpSocket->errorString();
        emit errorOccurred(errorString);
        return false;
    }

    connect(mTcpSocket, &QTcpSocket::disconnected, this, [=](){
        emit errorOccurred(QString("Connection has been closed!"));
    });
    connect(mTcpSocket,
#if QT_VERSION > QT_VERSION_CHECK(5, 14, 0)
            &QTcpSocket::errorOccurred,
#else
            QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
#endif
            this, [=](QAbstractSocket::SocketError err){
        if (err != QAbstractSocket::RemoteHostClosedError) {
            emit errorOccurred(mTcpSocket->errorString());
        }
    });

    return true;
}

SAKDebuggerDevice::ReadContextVector SAKTcpClientDevice::read()
{
    ReadContext ctx;
    ctx.bytes = mTcpSocket->readAll();
    ctx.flag = QString("%1:%2")
            .arg(mTcpSocket->peerAddress().toString())
            .arg(mTcpSocket->peerPort());
    return (ReadContextVector() << ctx);
}

SAKDebuggerDevice::WriteContext SAKTcpClientDevice::write(const QByteArray &bytes)
{
    WriteContext context;
    qint64 ret = mTcpSocket->write(bytes);
    context.flag = QString("%1:%2").arg(mTcpSocket->peerAddress().toString()).arg(mTcpSocket->peerPort());
    if (ret > 0){
        context.bytes = bytes;
    }
    return context;
}

void SAKTcpClientDevice::uninitialize()
{
    if (mTcpSocket->state() == QTcpSocket::ConnectedState){
        mTcpSocket->disconnectFromHost();
    }

    mTcpSocket->close();
    emit serverInfoChanged(QString());
    delete mTcpSocket;
    mTcpSocket = Q_NULLPTR;
}
