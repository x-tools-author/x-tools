/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <hidapi.h>

#include <QTimer>

#include "device.h"

struct HidDeviceParameterKeys
{
    const QString path{"path"};
};

class HidDevice : public Device
{
    Q_OBJECT
public:
    explicit HidDevice(QObject *parent = nullptr);
    ~HidDevice() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeActually(const QByteArray &bytes) override;

private:
    hid_device *m_hidDevice{nullptr};
    QTimer *m_readTimer{nullptr};

private:
    void readBytesFromDevice();
};
