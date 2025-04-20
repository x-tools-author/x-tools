/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "deviceui.h"

#include "device.h"

DeviceUi::DeviceUi(QWidget *parent)
    : QWidget(parent)
    , m_device(nullptr)
{}

DeviceUi::~DeviceUi()
{
    closeDevice();
}

void DeviceUi::openDevice()
{
    closeDevice();

    Device *device = this->device();
    device->load(save());
    device->openDevice();
}

void DeviceUi::closeDevice()
{
    if (m_device) {
        m_device->closeDevice();
    }
}

Device *DeviceUi::device()
{
    if (!m_device) {
        m_device = newDevice();
    }

    return m_device;
}

QVariantMap DeviceUi::save() const
{
    return QVariantMap();
}

void DeviceUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

QList<QWidget *> DeviceUi::deviceControllers()
{
    return QList<QWidget *>();
}

void DeviceUi::setUiEnabled(bool enabled)
{
    setEnabled(enabled);
}
