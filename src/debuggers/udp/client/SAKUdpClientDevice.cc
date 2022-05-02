/*
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
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
#if 0
        SAKStructAdvancedCtx ctx = advancedCtx();
        for (int i = 0; i < ctx.multicastCtxVector.length(); i++) {
            SAKStructMulticastCtx multicastCtx = ctx.multicastCtxVector.at(i);
            QHostAddress address(multicastCtx.address);
            if (!mUdpSocket->joinMulticastGroup(address)) {
                const QString info = QString("Can not join multicast group");
                emit errorOccurred(info);
            }
        }
#endif
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

SAKDebuggerDevice::ReadContextVector SAKUdpClientDevice::read()
{
    ReadContextVector contexts;
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(static_cast<int>(mUdpSocket->pendingDatagramSize()));
        QHostAddress host;
        quint16 port;
        qint64 ret = mUdpSocket->readDatagram(data.data(), data.length(), &host, &port);
        if (ret > 0){
            ReadContext ctx;
            ctx.bytes = data;
            ctx.flag = host.toString() + ":" + QString::number(port);
            contexts.push_back(ctx);
        }
    }

    return contexts;
}

SAKDebuggerDevice::WriteContext SAKUdpClientDevice::write(const QByteArray &bytes)
{
    WriteContext context;
    SAKStructAdvancedCtx ctx = advancedCtx();
    qint64 ret = 0;

    if (ctx.enable) {
        if (ctx.enableUnicast) {
            for (int i = 0; i < ctx.unicastCtxVector.length(); i++) {
                SAKStructAddressCtx info = ctx.unicastCtxVector.at(i);
                qint64 ret = mUdpSocket->writeDatagram(
                            bytes, QHostAddress(info.address), info.port);
                if (ret > 0) {
                    emit bytesWritten(bytes, info.address + ":"
                                      + QString::number(info.port));
                } else {
                    qWarning() << QString("Write datagram to %1:%2 failed!")
                                  .arg(info.address).arg(info.port);
                }
            }
        }

        if (ctx.enableMulticast) {
            for (int i = 0; i < ctx.multicastCtxVector.length(); i++) {
                SAKStructAddressCtx info = ctx.multicastCtxVector.at(i);
                qint64 ret = mUdpSocket->writeDatagram(
                            bytes, QHostAddress(info.address), info.port);
                if (ret > 0) {
                    emit bytesWritten(bytes, info.address + ":"
                                      + QString::number(info.port));
                } else {
                    qWarning() << QString("Write datagram to %1:%2 failed!")
                                  .arg(info.address).arg(info.port);
                }
            }
        }

        if (ctx.enableBroadcast) {
            for (int i = 0; i < ctx.broadcastCtxVector.length(); i++) {
                quint16 port = ctx.broadcastCtxVector.at(i);
                qint64 ret = mUdpSocket->writeDatagram(
                            bytes, QHostAddress::Broadcast, port);
                if (ret > 0) {
                    emit bytesWritten(bytes, "255.255.255.255:"
                                      + QString::number(port));
                } else {
                    const QString host =
                            QHostAddress(QHostAddress::Broadcast).toString();
                    qWarning() << QString("Write datagram to %1:%2 failed!")
                                  .arg(host).arg(port);
                }
            }
        }
    }

    auto parameters =
            parametersContext().value<SAKUdpClientParametersContext>();
    if (!parameters.pauseSending) {
        ret = mUdpSocket->writeDatagram(bytes,
                                        QHostAddress(parameters.peerHost),
                                        parameters.peerPort);

        if (ret > 0){
            const QString flag = parameters.peerHost + ":"
                    + QString::number(parameters.peerPort);
            emit bytesWritten(bytes, flag);
        } else {
            QString msg = tr("Write data failed:");
            emit invokeOutputMessage(msg + mUdpSocket->errorString());
        }
    }

    return context;
}

void SAKUdpClientDevice::uninitialize()
{
    mUdpSocket->close();
    delete mUdpSocket;
    mUdpSocket = Q_NULLPTR;
}

void SAKUdpClientDevice::setAdvancedCtx(const QJsonObject &parameters)
{
    SAKStructAdvancedCtx ctx;
    bool enable = parameters.value("enable").toBool();
    bool enableUnicast = parameters.value("enableUnicast").toBool();
    bool enableMulticast = parameters.value("enableMulticast").toBool();
    bool enableBoradcast = parameters.value("enableBoradcast").toBool();

    ctx.enable = enable;
    ctx.enableUnicast = enableUnicast;
    ctx.enableMulticast = enableMulticast;
    ctx.enableBroadcast = enableBoradcast;

    QJsonArray unicastArray = parameters.value("unicastCtx").toArray();
    QJsonArray multicastArray = parameters.value("multicastCtx").toArray();
    QJsonArray broadcastArray = parameters.value("broadcastCtx").toArray();

    for (int i = 0; i < unicastArray.count(); i++) {
        QJsonObject jsonObj = unicastArray.at(i).toObject();
        bool enable = jsonObj.value("enable").toBool();
        if (enable) {
            const QString ctxString = jsonObj.value("value").toString();
            const QStringList ctxList = ctxString.split(':');
            if (ctxList.length() == 2) {
                const QString address = ctxList.first();
                const quint16 port = ctxList.last().toUInt();
                SAKStructAddressCtx addressCtx{address, port};
                ctx.unicastCtxVector.append(addressCtx);
            }

        }
    }

    for (int i = 0; i < multicastArray.count(); i++) {
        QJsonObject jsonObj = multicastArray.at(i).toObject();
        bool enable = jsonObj.value("enable").toBool();
        if (enable) {
            const QString ctxString = jsonObj.value("value").toString();
            const QStringList ctxList = ctxString.split(':');
            if (ctxList.length() == 2) {
                const QString address = ctxList.first();
                const quint16 port = ctxList.last().toUInt();
                SAKStructAddressCtx addressCtx{address, port};
                ctx.multicastCtxVector.append(addressCtx);
            }
        }
    }

    for (int i = 0; i < broadcastArray.count(); i++) {
        QJsonObject jsonObj = broadcastArray.at(i).toObject();
        bool enable = jsonObj.value("enable").toBool();
        if (enable) {
            const quint16 port = jsonObj.value("value").toInt();
            ctx.broadcastCtxVector.append(port);
        }
    }

    mAdvancedCtxMutex.lock();
    mAdvancedCtx = ctx;
    mAdvancedCtxMutex.unlock();
}

SAKUdpClientDevice::SAKStructAdvancedCtx SAKUdpClientDevice::advancedCtx()
{
    mAdvancedCtxMutex.lock();
    SAKStructAdvancedCtx ctx = mAdvancedCtx;
    mAdvancedCtxMutex.unlock();

    return ctx;
}
