/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include "sakbroadcastthread.h"

#include <QTimer>
#include <QUdpSocket>

SAKBroadcastThread::SAKBroadcastThread(QObject* parent) : QThread{parent} {}

SAKBroadcastThread::~SAKBroadcastThread() {
  if (isRunning()) {
    exit();
    wait();
  }
}

void SAKBroadcastThread::SetBroadcastInformation(const QString& address,
                                                 quint16 port, int interval,
                                                 const QByteArray& data) {
  parameters_mutext_.lock();
  parameters_.address = address;
  parameters_.port = port;
  parameters_.interval = interval;
  parameters_.data = data;
  parameters_mutext_.unlock();
}

void SAKBroadcastThread::run() {
  QUdpSocket* udp_socket = new QUdpSocket();
  if (!udp_socket->bind()) {
    qWarning() << udp_socket->errorString();
    return;
  }

  parameters_mutext_.lock();
  auto parameters = parameters_;
  parameters_mutext_.unlock();

  QTimer* tx_timer = new QTimer();
  tx_timer->setSingleShot(true);
  tx_timer->setInterval(parameters.interval < 20 ? 20 : parameters.interval);
  connect(tx_timer, &QTimer::timeout, tx_timer, [=]() {
    qint64 ret = udp_socket->writeDatagram(
        parameters.data, QHostAddress(parameters.address), parameters.port);
    if (ret < 0) {
      qWarning() << udp_socket->error();
    } else {
      emit BytesWritten(parameters.data);
    }

    tx_timer->start();
  });
  tx_timer->start();

  exec();

  tx_timer->stop();
  tx_timer->deleteLater();
  tx_timer = Q_NULLPTR;

  udp_socket->close();
  udp_socket->deleteLater();
  udp_socket = Q_NULLPTR;
}
