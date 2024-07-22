/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Device.h"

Device::Device(QObject *parent)
    : AbstractIO(parent)
{}

Device::~Device()
{
    if (isRunning()) {
        closeDevice();
    }
}

void Device::openDevice()
{
    if (isRunning()) {
        closeDevice();
    }

    start();
}

void Device::closeDevice()
{
    exit();
    wait();
}

void Device::writeBytes(const QByteArray &bytes)
{
    emit invokeWriteBytes(bytes);
}

void Device::setParameters(const QVariantMap &parameters)
{
    m_parametersMutex.lock();
    m_parameters = parameters;
    m_parametersMutex.unlock();
}

void Device::run()
{
    m_deviceObj = initDevice();
    if (!m_deviceObj) {
        emit closed();
        return;
    }

    connect(this, &Device::invokeWriteBytes, m_deviceObj, [this](const QByteArray &bytes) {
        writeBytesToDevice(bytes);
    });

    emit opened();
    exec();

    m_deviceObj = nullptr;
    deinitDevice();
    emit closed();
}
