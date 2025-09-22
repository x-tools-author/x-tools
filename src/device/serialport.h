/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSerialPort>
#include <QTimer>

#include "device.h"

class SerialPort : public Device
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeActually(const QByteArray &bytes) override;

private:
    QSerialPort *m_serialPort{nullptr};

private:
    void readBytesFromDevice(bool optimizedFrame);
    void readBytesFromDeviceNormal();
    void readBytesFromDeviceOptimized();
    int calculateInterFrameDelay();
};
