/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKTcpClientTool.hh"

SAKTcpClientTool::SAKTcpClientTool(QObject *parent)
    : SAKSocketClientTool{"SAK.TcpClientTool", parent}
{

}

bool SAKTcpClientTool::initialize()
{
    mTcpSocket = new QTcpSocket();
    if (mSpecifyClientIpPort) {
        if (!mTcpSocket->bind(QHostAddress(mClientIp), mClientPort)) {
            outputMessage(QtWarningMsg, "Binding error:" + mTcpSocket->errorString());
            return false;
        }
    }

    mTcpSocket->connectToHost(QHostAddress(mServerIp), mServerPort);
    if (!mTcpSocket->waitForConnected()) {
        qDebug() << mServerIp << mServerPort;
        outputMessage(QtWarningMsg, "Connect to host error:" + mTcpSocket->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mTcpSocket->localAddress().toString()).arg(mTcpSocket->localPort());
    outputMessage(QtInfoMsg, info);

    mBindingIpPort = info;
    emit bindingIpPortChanged();

    connect(mTcpSocket, &QTcpSocket::errorOccurred, mTcpSocket, [=](QAbstractSocket::SocketError err){
        Q_UNUSED(err);
        emit errorOccured(mTcpSocket->errorString());
        outputMessage(QtWarningMsg, "Error occurred:" + mTcpSocket->errorString());
        exit();
    });

    connect(mTcpSocket, &QTcpSocket::readyRead, mTcpSocket, [=](){
        readBytes();
    });

    return true;
}

void SAKTcpClientTool::writeBytes(const QByteArray &bytes, const QVariant &context)
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
        QString info = QString("%1:%2")
                           .arg(address.toString())
                           .arg(port);
        emit bytesOutputted(bytes, info);
        Q_UNUSED(info);
    }
}
