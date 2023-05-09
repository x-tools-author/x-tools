/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDTcpClientTool.hpp"

EDTcpClientTool::EDTcpClientTool(QObject *parent)
    : EDBaseTool{"ED.TcpClientTool", parent}
{

}

bool EDTcpClientTool::initialize(QString &errStr)
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
    setToolFlag(info);

    mBindingIpPort = info;
    emit bindingIpPortChanged();

    connect(mTcpSocket, &QTcpSocket::errorOccurred, mTcpSocket, [=](QAbstractSocket::SocketError err){
        Q_UNUSED(err);
        emit errorOccured(toolFlag(), mTcpSocket->errorString());
        outputMessage(QtWarningMsg, "Error occurred:" + mTcpSocket->errorString());
        exit();
    });

    connect(mTcpSocket, &QTcpSocket::readyRead, mTcpSocket, [=](){
        emit invokeOutputBytes(EDBaseTool::EDPrivateSignal{});
    });

    return true;
}

void EDTcpClientTool::inputBytesHandler(const QByteArray &bytes)
{
    qint64 ret = mTcpSocket->write(bytes);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mTcpSocket->errorString());
    } else {
        emit bytesInputted(toolFlag(), bytes);
    }
}

void EDTcpClientTool::outputBytesHandler()
{
    QHostAddress address = mTcpSocket->peerAddress();
    quint16 port = mTcpSocket->peerPort();
    QByteArray bytes = mTcpSocket->readAll();
    if (!bytes.isEmpty()) {
        QString info = QString("%1:%2")
                           .arg(address.toString())
                           .arg(port);
        emit bytesOutputted(info, bytes);
        Q_UNUSED(info);
    }
}

void EDTcpClientTool::uninitialize()
{
    mTcpSocket->deleteLater();
    mTcpSocket = nullptr;
}
