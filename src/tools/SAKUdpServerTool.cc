/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKInterface.hh"
#include "SAKCompatibility.hh"
#include "SAKUdpServerTool.hh"

SAKUdpServerTool::SAKUdpServerTool(QObject *parent)
    : SAKSocketServerTool{"sak.udpservertool", parent}
{

}

bool SAKUdpServerTool::initialize(QString &errStr)
{
    mUdpSocket = new QUdpSocket();
    bool ret = false;
    if (mSpecifyIpAndPort) {
        ret = mUdpSocket->bind(QHostAddress(mServerIp), mServerPort);
    } else {
        ret = mUdpSocket->bind();
    }

    if (!ret) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
        errStr = mUdpSocket->errorString();
        return false;
    }

    QString ip = mUdpSocket->localAddress().toString();
    int port = mUdpSocket->localPort();
    mBindingIpPort = QString("%1:%2").arg(ip).arg(port);
    outputMessage(QtInfoMsg, mBindingIpPort);
    emit bindingIpPortChanged();

    connect(mUdpSocket, &QUdpSocket::readyRead,
            mUdpSocket, [=](){readBytes();});
    connect(mUdpSocket, SAK_SOCKET_ERROR, this, [=](){
        emit errorOccured(mUdpSocket->errorString());
    });

    return true;
}

void SAKUdpServerTool::writeBytes(const QByteArray &bytes,
                                  const QVariant &context)
{
    if (mClientIndex >= 0 && mClientIndex < mClients.length()) {
        QString ipPort = mClients.at(mClientIndex);
        QStringList list = ipPort.split(":");
        QString ip = list.first();
        quint16 port = list.last().toInt();
        writeDatagram(bytes, context, ip, port);
    } else {
        for (auto &client : mClients) {
            QStringList list = client.split(":");
            QString ip = list.first();
            quint16 port = list.last().toInt();
            writeDatagram(bytes, context, ip, port);
        }
    }
}

void SAKUdpServerTool::uninitialize()
{
    mUdpSocket->deleteLater();
    mUdpSocket = nullptr;
}

void SAKUdpServerTool::readBytes()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        auto len = mUdpSocket->pendingDatagramSize();
        if (len == -1) {
            break;
        }

        QByteArray bytes(len, 0);
        QHostAddress address;
        quint16 port;
        qint64 ret = mUdpSocket->readDatagram(bytes.data(), bytes.length(),
                                              &address, &port);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mUdpSocket->errorString());
            break;
        }

        QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
        QString hex = QString::fromLatin1(ba);
        QString info =
            QString("%1:%2").arg(address.toString()).arg(port);
        QString msg =
            QString("%1<-%2:%3").arg(mBindingIpPort, info, hex);
        outputMessage(QtInfoMsg, msg);

        if (!mClients.contains(info)) {
            mClients.append(info);
            emit clientsChanged();
        }

        emit bytesOutputted(bytes, info);
    }
}

void SAKUdpServerTool::writeDatagram(const QByteArray &bytes,
                                     const QVariant &context,
                                     const QString &ip, quint16 port)
{
    qint64 ret = mUdpSocket->writeDatagram(bytes, QHostAddress(ip), port);
    if (ret == -1) {
        outputMessage(QtWarningMsg, mUdpSocket->errorString());
    } else {
        QString hex = QString::fromLatin1(SAKInterface::arrayToHex(bytes, ' '));
        outputMessage(QtInfoMsg,
                      QString("%1<-%2").arg(mBindingIpPort, hex));
        emit bytesInputted(bytes, context);
    }
}
