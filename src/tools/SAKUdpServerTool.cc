/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDUdpServerTool.hpp"

EDUdpServerTool::EDUdpServerTool(QObject *parent)
    : EDBaseTool{"ED.UdpServerTool", parent}
{

}

bool EDUdpServerTool::initialize(QString &errStr)
{
    mUdpSocket = new QUdpSocket();
    if (!mUdpSocket->bind(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mUdpSocket->localAddress().toString()).arg(mUdpSocket->localPort());
    outputMessage(QtInfoMsg, info);
    setToolFlag(info);

    connect(mUdpSocket, &QUdpSocket::readyRead, mUdpSocket, [=](){
        emit invokeOutputBytes(EDBaseTool::EDPrivateSignal{});
    });

    return true;
}

void EDUdpServerTool::inputBytesHandler(const QByteArray &bytes)
{
    if (mClientIndex >= 0 && mClientIndex < mClients.length()) {
        QString ipPort = mClients.at(mClientIndex);
        int index = ipPort.lastIndexOf(":");
        QString ip = ipPort.first(index);
        quint16 port = ipPort.last(ipPort.length() - index - 1).toInt();
        qInfo() << "client ip:" << ip << ";" << "client port:" << port;
        qint64 ret = mUdpSocket->writeDatagram(bytes, QHostAddress(ip), port);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mUdpSocket->errorString());
        } else {
            emit bytesInputted(toolFlag(), bytes);
        }
    }
}

void EDUdpServerTool::outputBytesHandler()
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
                if (!mClients.contains(info)) {
                    mClients.append(info);
                    emit clientsChanged();
                }

                emit bytesOutputted(info, bytes);
            }
        }
    }
}

void EDUdpServerTool::uninitialize()
{
    mUdpSocket->deleteLater();
    mUdpSocket = nullptr;
}
