/*
 * Copyright 2020-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QJsonArray>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>

#include "SAKDebugger.hh"
#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugger.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKUdpServerController.hh"

SAKUdpServerDevice::SAKUdpServerDevice(QSettings *settings,
                                       const QString &settingsGroup,
                                       QWidget *uiParent,
                                       QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mUdpServer(Q_NULLPTR)
{

}

void SAKUdpServerDevice::setMulticastParameters(const QJsonArray &parameters)
{
    mMulticastHostListMutex.lock();
    mMulticastHostList.clear();
    for (int i = 0; i < parameters.count(); i++) {
        QJsonObject jsonObj = parameters.at(i).toObject();
        bool enable = jsonObj.value("enable").toBool();
        if (enable) {
            const QString host = jsonObj.value("value").toString();
            mMulticastHostList.append(host);
        }
    }
    mMulticastHostListMutex.unlock();
}

bool SAKUdpServerDevice::initialize()
{
    auto parameters = parametersContext().value<SAKUdpServerParametersContext>();
    mUdpServer = new QUdpSocket;
    if (!mUdpServer->bind(QHostAddress(parameters.serverHost),
                          parameters.serverPort,
                          QUdpSocket::ShareAddress)) {
        QString errorString = tr("Binding failed：") + mUdpServer->errorString();
        emit errorOccurred(errorString);
        return false;
    } else {
        QStringList hosts = multicastParameters();
        for (int i = 0; i < hosts.length(); i++) {
            const QString host = hosts.at(i);
            if (!mUdpServer->joinMulticastGroup(QHostAddress(host))) {
                qDebug() << QString("Join multicast group(%1) filed:")
                            .arg(host);
            }
        }
        connect(mUdpServer, &QUdpSocket::readyRead,
                this, [=](){
            emit readyRead(SAKDeviceProtectedSignal());
        }, Qt::DirectConnection);
    }

    return true;
}

SAKDebuggerDevice::ReadContextVector SAKUdpServerDevice::read()
{
    ReadContextVector contexts;
    while (mUdpServer->hasPendingDatagrams()) {
        qint64 size = mUdpServer->pendingDatagramSize();
        QByteArray bytes;
        bytes.resize(size);
        QHostAddress peerAddress;
        quint16 peerPort;
        qint64 ret = mUdpServer->readDatagram(bytes.data(),
                                              size,
                                              &peerAddress,
                                              &peerPort);
        if (ret > 0) {
            auto parameters = parametersContext().value<SAKUdpServerParametersContext>();
            QString currentHost = parameters.currentClientHost;
            quint16 currentPort = parameters.currentClientPort;
            const QString flag = peerAddress.toString() + ":"
                    + QString::number(peerPort);
            if (currentHost.isEmpty()) {
                emit addClient(peerAddress.toString(), peerPort);
                ReadContext ctx;
                ctx.bytes = bytes;
                ctx.flag = flag;
                contexts.push_back(ctx);
            } else {
                QStringList clients = parameters.clients;
                if (!clients.contains(flag)) {
                    emit addClient(peerAddress.toString(), peerPort);
                }

                if ((currentHost == peerAddress.toString())
                        && (currentPort == peerPort)) {
                    ReadContext ctx;
                    ctx.bytes = bytes;
                    ctx.flag = flag;
                    contexts.push_back(ctx);
                }
            }
        }
    }

    return contexts;
}

SAKDebuggerDevice::WriteContext SAKUdpServerDevice::write(const QByteArray &bytes)
{
    WriteContext context;
    auto parameters = parametersContext().value<SAKUdpServerParametersContext>();
    QString currentHost = parameters.currentClientHost;
    quint16 currentPort = parameters.currentClientPort;
    qint64 ret = mUdpServer->writeDatagram(bytes,
                                           QHostAddress(currentHost),
                                           currentPort);
    if (ret > 0){
        context.bytes = bytes;
    }
    return context;
}

void SAKUdpServerDevice::uninitialize()
{
    mUdpServer->close();
    delete mUdpServer;
    mUdpServer = Q_NULLPTR;
}

QStringList SAKUdpServerDevice::multicastParameters()
{
    mMulticastHostListMutex.lock();
    QStringList list = mMulticastHostList;
    mMulticastHostListMutex.unlock();

    return list;
}
