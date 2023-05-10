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
#include "SAKUdpClientTool.hh"

SAKUdpClientTool::SAKUdpClientTool(QObject *parent)
    : SAKCommunicationTool{"SAK.UdpClientTool", parent}
{

}

bool SAKUdpClientTool::initialize()
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

    mBindingIpPort = info;
    emit bindingIpPortChanged();

    connect(mUdpSocket, &QUdpSocket::readyRead, mUdpSocket, [=](){
        readBytes();
    });

    return true;
}

void SAKUdpClientTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    Q_UNUSED(context);
    qint64 ret = mUdpSocket->writeDatagram(bytes, QHostAddress(mServerIp), mServerPort);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
    } else {
        emit bytesInputted(bytes, QVariant());
    }
}

void SAKUdpClientTool::readBytes()
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
                emit bytesOutputted(bytes, QVariant());
                Q_UNUSED(info);
            }
        }
    }
}

void SAKUdpClientTool::uninitialize()
{
    mUdpSocket->deleteLater();
    mUdpSocket = nullptr;
}
