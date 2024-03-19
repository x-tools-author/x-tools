/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakbroadcastthread.h"

#include <QTimer>
#include <QUdpSocket>

SAKBroadcastThread::SAKBroadcastThread(QObject* parent)
    : QThread{parent}
{}

SAKBroadcastThread::~SAKBroadcastThread()
{
    if (isRunning()) {
        exit();
        wait();
    }
}

void SAKBroadcastThread::setBroadcastInformation(const QString& address,
                                                 quint16 port,
                                                 int interval,
                                                 const QByteArray& data)
{
    m_parametersMutext.lock();
    m_parameters.address = address;
    m_parameters.port = port;
    m_parameters.interval = interval;
    m_parameters.data = data;
    m_parametersMutext.unlock();
}

void SAKBroadcastThread::run()
{
    QUdpSocket* udpSocket = new QUdpSocket();
    if (!udpSocket->bind()) {
        qWarning() << udpSocket->errorString();
        return;
    }

    m_parametersMutext.lock();
    auto parameters = m_parameters;
    m_parametersMutext.unlock();

    QTimer* txTimer = new QTimer();
    txTimer->setSingleShot(true);
    txTimer->setInterval(parameters.interval < 20 ? 20 : parameters.interval);
    connect(txTimer, &QTimer::timeout, txTimer, [=]() {
        QHostAddress hostAddress(parameters.address);
        qint64 ret = udpSocket->writeDatagram(parameters.data, hostAddress, parameters.port);
        if (ret < 0) {
            qWarning() << udpSocket->error();
        } else {
            emit bytesWritten(parameters.data);
        }

        txTimer->start();
    });
    txTimer->start();

    exec();

    txTimer->stop();
    txTimer->deleteLater();
    txTimer = Q_NULLPTR;

    udpSocket->close();
    udpSocket->deleteLater();
    udpSocket = Q_NULLPTR;
}
