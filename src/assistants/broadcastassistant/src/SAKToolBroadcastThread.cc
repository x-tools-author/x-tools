/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include <QTimer>
#include <QUdpSocket>
#include "SAKToolBroadcastThread.hh"

SAKToolBroadcastThread::SAKToolBroadcastThread(QObject *parent)
    : QThread{parent}
{

}

SAKToolBroadcastThread::~SAKToolBroadcastThread()
{
    if (isRunning()) {
        exit();
        wait();
    }
}

void SAKToolBroadcastThread::setBroadcastInformation(const QString &address,
                                                     quint16 port,
                                                     int interval,
                                                     const QByteArray &data)
{
    mParametersMutext.lock();
    mParameters.address = address;
    mParameters.port = port;
    mParameters.interval = interval;
    mParameters.data = data;
    mParametersMutext.unlock();
}

void SAKToolBroadcastThread::run()
{
    QUdpSocket *udpSocket = new QUdpSocket();
    if (!udpSocket->bind()) {
        qWarning() << udpSocket->errorString();
        return;
    }

    mParametersMutext.lock();
    auto parameters = mParameters;
    mParametersMutext.unlock();

    QTimer *txTimer = new QTimer();
    txTimer->setSingleShot(true);
    txTimer->setInterval(parameters.interval < 20 ? 20 : parameters.interval);
    connect(txTimer, &QTimer::timeout, txTimer, [=](){
        qint64 ret = udpSocket->writeDatagram(parameters.data,
                                              QHostAddress(parameters.address),
                                              parameters.port);
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
