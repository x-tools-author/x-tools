/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientDebugger.hh"
#include "SAKUdpClientController.hh"

SAKUdpClientDevice::SAKUdpClientDevice(QSettings *settings,
                                       const QString &settingsGroup,
                                       QWidget *uiParent,
                                       QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mUdpSocket(Q_NULLPTR)
{

}

SAKUdpClientDevice::~SAKUdpClientDevice()
{

}

bool SAKUdpClientDevice::initialize()
{
    auto parameters = parametersContext().value<SAKUdpClientParametersContext>();
    bool specifyLocalInfo = parameters.specifyLocalInfo;
    QString localHost = parameters.localHost;
    quint16 localPort = parameters.localPort;


    // Binding host and port.
    mUdpSocket = new QUdpSocket;
    bool bindResult = false;
    if (specifyLocalInfo) {
        if (localHost.compare(SAK_HOST_ADDRESS_ANY) == 0) {
            bindResult = mUdpSocket->bind(QHostAddress::Any,
                                          localPort,
                                          QUdpSocket::ShareAddress);
        } else {
            bindResult = mUdpSocket->bind(QHostAddress(localHost),
                                          localPort,
                                          QUdpSocket::ShareAddress);
        }
    } else {
        bindResult = mUdpSocket->bind();
    }


    // Check binding result.
    if (bindResult){
        QString info = mUdpSocket->localAddress().toString();
        info.append(":");
        info.append(QString::number(mUdpSocket->localPort()));
        emit clientInfoChanged(info);
    }else{
        QString errorString = tr("Binding failed:") + mUdpSocket->errorString();
        emit errorOccurred(errorString);
        return false;
    }


    // Open socket.
    if (mUdpSocket->open(QUdpSocket::ReadWrite)){
        connect(mUdpSocket, &QUdpSocket::readyRead, this, [=](){
            emit readyRead(SAKDeviceProtectedSignal());
        }, Qt::DirectConnection);
    } else {
        QString errorString = tr("Open device failed:") + mUdpSocket->errorString();
        emit errorOccurred(errorString);
        return false;
    }


    return true;
}

QByteArray SAKUdpClientDevice::read()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(static_cast<int>(mUdpSocket->pendingDatagramSize()));
        qint64 ret = mUdpSocket->readDatagram(data.data(), data.length());
        if (ret > 0){
            emit bytesRead(data);
        }
    }

    return QByteArray();
}

QByteArray SAKUdpClientDevice::write(const QByteArray &bytes)
{
    auto parameters = parametersContext().value<SAKUdpClientParametersContext>();
    qint64 ret = mUdpSocket->writeDatagram(bytes,
                                           QHostAddress(parameters.peerHost),
                                           parameters.peerPort);
    if (ret > 0){
        emit bytesWritten(bytes);
    }

    return QByteArray();
}

void SAKUdpClientDevice::uninitialize()
{
    mUdpSocket->close();
    delete mUdpSocket;
    mUdpSocket = Q_NULLPTR;
}
