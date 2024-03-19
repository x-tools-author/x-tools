/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKBROADCASTTHREAD_H
#define SAKBROADCASTTHREAD_H

#include <QMutex>
#include <QThread>

class SAKBroadcastThread : public QThread
{
    Q_OBJECT
public:
    explicit SAKBroadcastThread(QObject* parent = Q_NULLPTR);
    ~SAKBroadcastThread();

    void setBroadcastInformation(const QString& address,
                                 quint16 port,
                                 int interval,
                                 const QByteArray& data);

signals:
    void bytesWritten(const QByteArray& bytes);

protected:
    void run() final;

private:
    struct Parameters
    {
        QString address;
        quint16 port;
        int interval;
        QByteArray data;
    } m_parameters;

private:
    QMutex m_parametersMutext;
};

#endif // SAKBROADCASTTHREAD_H
