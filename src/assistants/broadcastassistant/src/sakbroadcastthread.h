/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBROADCASTTHREAD_H
#define SAKBROADCASTTHREAD_H

#include <QMutex>
#include <QThread>

class SAKBroadcastThread : public QThread {
  Q_OBJECT
 public:
  explicit SAKBroadcastThread(QObject* parent = Q_NULLPTR);
  ~SAKBroadcastThread();

  void SetBroadcastInformation(const QString& address, quint16 port,
                               int interval, const QByteArray& data);

 signals:
  void BytesWritten(const QByteArray& bytes);

 protected:
  void run() final;

 private:
  struct Parameters {
    QString address;
    quint16 port;
    int interval;
    QByteArray data;
  } parameters_;

 private:
  QMutex parameters_mutext_;
};

#endif  // SAKBROADCASTTHREAD_H
