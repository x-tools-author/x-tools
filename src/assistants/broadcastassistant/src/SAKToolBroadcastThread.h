/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBROADCASTTHREAD_H
#define SAKTOOLBROADCASTTHREAD_H

#include <QMutex>
#include <QThread>

class SAKToolBroadcastThread : public QThread
{
    Q_OBJECT
public:
    explicit SAKToolBroadcastThread(QObject *parent = Q_NULLPTR);
    ~SAKToolBroadcastThread();

    void setBroadcastInformation(const QString &address, quint16 port,
                                 int interval, const QByteArray &data);

signals:
    void bytesWritten(const QByteArray &bytes);

protected:
    void run() final;

private:
    struct Parameters {
        QString address;
        quint16 port;
        int interval;
        QByteArray data;
    } mParameters;

private:
    QMutex mParametersMutext;
};

#endif // SAKTOOLBROADCASTTHREAD_H
