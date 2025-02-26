/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QHid>
#include <QTimer>

#include "Communication.h"

class Hid : public Communication
{
    Q_OBJECT
public:
    explicit Hid(QObject *parent = nullptr);
    ~Hid() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeBytes(const QByteArray &bytes) override;

private:
    static constexpr int s_recommendedDelay = 2;
    QHid *m_Hid{nullptr};
    int m_interFrameDelayMilliseconds = s_recommendedDelay;
    QByteArray m_frameBuffer;
    QTimer *m_interFrameTimer{nullptr};
    QElapsedTimer m_interFrameTimerElapsed;

private:
    void readBytesFromDevice();
    void calculateInterFrameDelay();
};
