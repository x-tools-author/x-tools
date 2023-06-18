/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QHostAddress>

#include "SAKInterface.hh"
#include "SAKTcpClientTool.hh"

#define SOCKET_ERROR_SIG void(QAbstractSocket::*)(QAbstractSocket::SocketError)

SAKTcpClientTool::SAKTcpClientTool(QObject *parent)
    : SAKSocketClientTool{"SAK.TcpClientTool", parent}
{

}

bool SAKTcpClientTool::initialize(QString &errStr)
{
    mTcpSocket = new QTcpSocket();
    if (mSpecifyClientIpPort) {
        if (!mTcpSocket->bind(QHostAddress(mClientIp), mClientPort)) {
            errStr = "Binding error:" + mTcpSocket->errorString();
            outputMessage(QtWarningMsg, errStr);
            return false;
        }
    }

    mTcpSocket->connectToHost(QHostAddress(mServerIp), mServerPort);
    if (!mTcpSocket->waitForConnected()) {
        errStr = "Connect to host error:" + mTcpSocket->errorString();
        outputMessage(QtWarningMsg, errStr);
        return false;
    }

    QString ip = mTcpSocket->localAddress().toString();
    QString port = QString::number(mTcpSocket->localPort());
    mBindingIpPort = ip + ":" + port;
    outputMessage(QtInfoMsg, mBindingIpPort);
    emit bindingIpPortChanged();

    connect(mTcpSocket,
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            &QTcpSocket::errorOccurred,
#else
            static_cast<SOCKET_ERROR_SIG>(&QAbstractSocket::error),
#endif
            mTcpSocket, [=](QAbstractSocket::SocketError err){
        Q_UNUSED(err);

        QString info = "Error occurred:" + mTcpSocket->errorString();
        outputMessage(QtWarningMsg, info);
        emit errorOccured(mTcpSocket->errorString());
    });

    connect(mTcpSocket, &QTcpSocket::disconnected, mTcpSocket, [=](){
        exit();
    });

    connect(mTcpSocket, &QTcpSocket::readyRead, mTcpSocket, [=](){
        readBytes();
    });

    connect(mTcpSocket,
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        &QTcpSocket::errorOccurred,
#else
        static_cast<SOCKET_ERROR_SIG>(&QAbstractSocket::error),
#endif
        mTcpSocket, [=](){
            emit errorOccured(mTcpSocket->errorString());
    });

    return true;
}

void SAKTcpClientTool::writeBytes(const QByteArray &bytes,
                                  const QVariant &context)
{
    Q_UNUSED(context);
    qint64 ret = mTcpSocket->write(bytes);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mTcpSocket->errorString());
    } else {
        emit bytesInputted(bytes, QVariant());
    }
}

void SAKTcpClientTool::uninitialize()
{
    mTcpSocket->deleteLater();
    mTcpSocket = nullptr;
}

void SAKTcpClientTool::readBytes()
{
    QHostAddress address = mTcpSocket->peerAddress();
    quint16 port = mTcpSocket->peerPort();
    QByteArray bytes = mTcpSocket->readAll();
    if (!bytes.isEmpty()) {
        QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
        QString ipport = address.toString() + ":" + QString::number(port);
        QString info = mBindingIpPort + "<-" + ipport + ":";
        info += QString::fromLatin1(ba);
        outputMessage(QtDebugMsg, info);
        emit bytesOutputted(bytes, rxJsonObject());
    }
}
