/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "device.h"

#include <QDebug>

Device::Device(QObject *parent)
    : AbstractIO(parent)
{
    connect(this, &Device::started, this, [=]() {
        this->m_isWorking = true;
        emit this->isWorkingChanged();
    });
    connect(this, &Device::finished, this, [=]() {
        this->m_isWorking = false;
        emit this->isWorkingChanged();
    });
    connect(this, &Device::errorOccurred, this, [=](const QString &errorString) {
        qWarning() << "Error occured: " << errorString;
        exit();
        wait();
    });

    connect(this, &Device::bytesRead, this, [=](const QByteArray &bytes, const QString &) {
        emit outputBytes(bytes);
    });
}

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

void Device::run()
{
    QObject *obj = initDevice();
    if (!obj) {
        qWarning() << "Failed to init device";
        emit closed();
        return;
    }

    connect(this, &Device::invokeWriteBytes, obj, [this](const QByteArray &bytes) {
        writeBytes(bytes);
    });

    emit opened();
    exec();

    deinitDevice();
    emit closed();
}
