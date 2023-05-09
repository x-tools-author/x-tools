/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QHostAddress>
#include "EDUdpClientTool.hpp"

EDUdpClientTool::EDUdpClientTool(QObject *parent)
    : EDBaseTool{"ED.UdpClientTool", parent}
{

}

bool EDUdpClientTool::initialize(QString &errStr)
{
    mUdpSocket = new QUdpSocket();
    if (mSpecifyClientIpPort) {
        if (!mUdpSocket->bind(QHostAddress(mClientIp), mClientPort)) {
            outputMessage(QtWarningMsg, mUdpSocket->errorString());
            return false;
        }
    } else {
        mUdpSocket->bind();
    }

    QString info = QString("%1:%2").arg(mUdpSocket->localAddress().toString()).arg(mUdpSocket->localPort());
    outputMessage(QtInfoMsg, info);
    setToolFlag(info);

    mBindingIpPort = info;
    emit bindingIpPortChanged();

    connect(mUdpSocket, &QUdpSocket::readyRead, mUdpSocket, [=](){
        emit invokeOutputBytes(EDBaseTool::EDPrivateSignal{});
    });

    return true;
}

void EDUdpClientTool::inputBytesHandler(const QByteArray &bytes)
{
    qint64 ret = mUdpSocket->writeDatagram(bytes, QHostAddress(mServerIp), mServerPort);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
    } else {
        emit bytesInputted(toolFlag(), bytes);
    }
}

void EDUdpClientTool::outputBytesHandler()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        auto len = mUdpSocket->pendingDatagramSize();
        if (len != -1) {
            QByteArray bytes(len, 0);
            QHostAddress address;
            quint16 port;
            qint64 ret = mUdpSocket->readDatagram(bytes.data(), bytes.length(), &address, &port);
            if (ret == -1) {
                outputMessage(QtWarningMsg, mUdpSocket->errorString());
            } else {
                QString info = QString("%1:%2")
                                   .arg(address.toString())
                                   .arg(port);
                emit bytesOutputted(info, bytes);
                Q_UNUSED(info);
            }
        }
    }
}

void EDUdpClientTool::uninitialize()
{
    mUdpSocket->deleteLater();
    mUdpSocket = nullptr;
}
