/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Statistician.h"

#include <QTimer>

Statistician::Statistician(QObject *parent)
    : AbstractIO{parent}
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Statistician::updateSpeed);

    connect(this, &Statistician::started, this, [this, timer]() {
        this->m_frames = 0;
        this->m_bytes = 0;
        this->m_speed = 0;

        emit this->framesChanged();
        emit this->bytesChanged();
        emit this->speedChanged();

        timer->start();
    });

    connect(this, &Statistician::finished, this, [timer]() { timer->stop(); });
}

int Statistician::frames()
{
    return m_frames;
}

int Statistician::bytes()
{
    return m_bytes;
}

int Statistician::speed()
{
    return m_speed;
}

void Statistician::inputBytes(const QByteArray &bytes)
{
    if (isEnable()) {
        if (isWorking()) {
            m_frames++;
            m_bytes += bytes.size();

            emit framesChanged();
            emit bytesChanged();

            m_tempBytes.append(bytes);
        }
    } else {
        emit outputBytes(bytes);
    }
}

void Statistician::run()
{
    exec();
}

void Statistician::updateSpeed()
{
    m_speed = m_tempBytes.size();
    m_tempBytes.clear();
    emit speedChanged();
}
