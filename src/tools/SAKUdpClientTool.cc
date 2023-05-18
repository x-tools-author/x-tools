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
    : SAKSocketClientTool{"SAK.UdpClientTool", parent}
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

    QString ip = mUdpSocket->localAddress().toString();
    int port = mUdpSocket->localPort();
    QString info = QString("%1:%2").arg(ip).arg(port);
    mBindingIpPort = info;
    outputMessage(QtInfoMsg, info);
    emit bindingIpPortChanged();

    connect(mUdpSocket, &QUdpSocket::readyRead,
            mUdpSocket, [=](){readBytes();});

    return true;
}

void SAKUdpClientTool::writeBytes(const QByteArray &bytes,
                                  const QVariant &context)
{
    Q_UNUSED(context);
    qint64 ret = mUdpSocket->writeDatagram(bytes,
                                           QHostAddress(mServerIp),
                                           mServerPort);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
    } else {
        QString hex = QString::fromLatin1(bytes.toHex(' '));
        QString serverInfo = QString("%1:%2")
                                 .arg(mServerIp, QString::number(mServerPort));
        outputMessage(QtInfoMsg, QString("%1->%2:%3")
                                     .arg(mBindingIpPort, serverInfo, hex));
        emit bytesInputted(bytes, QVariant());
    }
}

void SAKUdpClientTool::uninitialize()
{
    mUdpSocket->deleteLater();
    mUdpSocket = nullptr;
}

void SAKUdpClientTool::readBytes()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        auto len = mUdpSocket->pendingDatagramSize();
        if (len != -1) {
            QByteArray bytes(len, 0);
            QHostAddress address;
            quint16 port;
            qint64 ret = mUdpSocket->readDatagram(bytes.data(),
                                                  bytes.length(),
                                                  &address, &port);
            if (ret == -1) {
                outputMessage(QtWarningMsg, mUdpSocket->errorString());
            } else {
                QString hex = QString::fromLatin1(bytes.toHex(' '));
                outputMessage(QtInfoMsg,
                              QString("%1<-%2").arg(mBindingIpPort, hex));
                emit bytesOutputted(bytes, QVariant());
            }
        }
    }
}
