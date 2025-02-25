/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QThread>

class BroadcastThread : public QThread
{
    Q_OBJECT
public:
    explicit BroadcastThread(QObject* parent = Q_NULLPTR);
    ~BroadcastThread();

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
