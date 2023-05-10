/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKUdpServerTool.hh"

SAKUdpServerTool::SAKUdpServerTool(QObject *parent)
    : SAKCommunicationTool{"SAK.UdpServerTool", parent}
{

}

bool SAKUdpServerTool::initialize()
{
    mUdpSocket = new QUdpSocket();
    if (!mUdpSocket->bind(QHostAddress(mServerIp), mServerPort)) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
        return false;
    }

    QString info = QString("%1:%2").arg(mUdpSocket->localAddress().toString()).arg(mUdpSocket->localPort());
    outputMessage(QtInfoMsg, info);

    connect(mUdpSocket, &QUdpSocket::readyRead, mUdpSocket, [=](){
        readBytes();
    });

    return true;
}

void SAKUdpServerTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    Q_UNUSED(context);
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
            emit bytesInputted(bytes, QVariant());
        }
    }
}

void SAKUdpServerTool::readBytes()
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

                emit bytesOutputted(bytes, info);
            }
        }
    }
}

void SAKUdpServerTool::uninitialize()
{
    mUdpSocket->deleteLater();
    mUdpSocket = nullptr;
}
