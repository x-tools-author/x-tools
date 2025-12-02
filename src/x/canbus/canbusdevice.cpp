/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "canbusdevice.h"

#include <QCanBus>

namespace xCanBus {

CanBusDevice::CanBusDevice(QObject *parent)
    : QThread(parent)
{}

CanBusDevice::~CanBusDevice()
{
    stopDevice();
}

void CanBusDevice::startDevice(const CanBusDeviceParameters &params)
{
    m_parametersMutex.lock();
    m_parameters = params;
    m_parametersMutex.unlock();
    start();
}

void CanBusDevice::stopDevice()
{
    requestInterruption();
    exit();
    wait();
}

void CanBusDevice::writeFrame(const QCanBusFrame &frame)
{
    emit invokeWriteFrame(frame);
}

void CanBusDevice::run()
{
    m_parametersMutex.lock();
    const CanBusDeviceParameters params = m_parameters;
    m_parametersMutex.unlock();

    QString errorMessage;
    QCanBusDevice *device = QCanBus::instance()->createDevice(params.plugin,
                                                              params.interfaceName,
                                                              &errorMessage);
    if (!device) {
        QString msg = tr("Failed to create CAN bus device:") + errorMessage;
        emit errorOccurred(msg);
        return;
    }

    for (const auto &pair : params.params) {
        device->setConfigurationParameter(pair.first, pair.second);
    }

    if (!device->connectDevice()) {
        QString msg = tr("Failed to connect to CAN bus device:") + device->errorString();
        emit errorOccurred(msg);
        device->deleteLater();
        return;
    }

    connect(device, &QCanBusDevice::errorOccurred, device, [this, device]() {
        QString msg = tr("CAN bus device error:") + device->errorString();
        emit errorOccurred(msg);
    });
    connect(device, &QCanBusDevice::framesReceived, device, [this, device]() {
        while (device->framesAvailable()) {
            QCanBusFrame frame = device->readFrame();
            emit frameRx(frame);
        }
    });
    connect(this, &CanBusDevice::invokeWriteFrame, device, [this, device](const QCanBusFrame &frame) {
        if (device->writeFrame(frame)) {
            emit frameTx(frame);
        }
    });

    emit opened();
    exec();
    device->disconnectDevice();
    device->deleteLater();
}

} // namespace xCanBus