/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "statistician.h"

#include <QTimer>

Statistician::Statistician(QLabel *view, QObject *parent)
    : QObject{parent}
    , m_frames{0}
    , m_bytes{0}
    , m_speed{0}
    , m_view{view}
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, [=]() {
        this->m_speed = m_tempBytes.size();
        this->m_tempBytes.clear();
        updateLabel();
    });
    timer->start();
}

void Statistician::inputBytes(const QByteArray &bytes)
{
    m_frames++;
    m_bytes += bytes.size();
    m_tempBytes.append(bytes);

    updateLabel();
}

void Statistician::reset()
{
    m_frames = 0;
    m_bytes = 0;
    m_speed = 0;
    m_tempBytes.clear();
    updateLabel();
}

void Statistician::updateLabel()
{
    if (m_view) {
        m_view->setText(tr("%1 frames, %2 bytes, %3B/s").arg(m_frames).arg(m_bytes).arg(m_speed));
    }
}