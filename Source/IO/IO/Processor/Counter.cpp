/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Counter.h"

#include <QTimer>

Counter::Counter(QObject *parent)
    : AbstractIO{parent}
{
    connect(this, &Counter::started, this, [this]() {
        this->m_frames = 0;
        this->m_bytes = 0;

        this->m_framesString = tr("0 Frames");
        this->m_bytesString = tr("0 Bytes");

        this->m_speedString = tr("0 Bytes/s");
        this->m_speed = 0;
    });
}

int Counter::frames()
{
    return m_frames;
}

int Counter::bytes()
{
    return m_bytes;
}

int Counter::speed()
{
    return m_speed;
}

QString Counter::framesString()
{
    return m_framesString;
}

QString Counter::bytesString()
{
    return m_bytesString;
}

QString Counter::speedString()
{
    return m_speedString;
}

void Counter::inputBytes(const QByteArray &bytes)
{
    if (isEnable()) {
        if (isWorking()) {
            m_speedMutex.lock();
            m_tempBytes.append(bytes);
            m_speedMutex.unlock();
        }
    } else {
        emit outputBytes(bytes);
    }
}

void Counter::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Counter::updateSpeed, Qt::DirectConnection);
    timer->start();

    exec();

    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}

void Counter::updateSpeed()
{
    m_speedMutex.lock();
    int v = m_tempBytes.size();
    m_speed = v;
    m_tempBytes.clear();
    m_speedMutex.unlock();

    QString speedString;
    if (v < 1024) {
        speedString = QString("%1Bytes/s").arg(v);
    } else if (v < 1024 * 1024) {
        speedString = QString("%1KB/s").arg(v / 1024);
    } else {
        speedString = QString("%1MB/s").arg(v / (1024 * 1024));
    }

    emit speedStringChanged();
}
