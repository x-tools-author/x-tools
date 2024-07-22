/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BleCentral.h"

BleCentral::BleCentral(QObject *parent)
    : Device(parent)
{}

BleCentral::~BleCentral() {}

QObject *BleCentral::initDevice()
{
    m_parametersMutex.lock();
    QBluetoothDeviceInfo info = m_parameters["deviceInfo"].value<QBluetoothDeviceInfo>();
    m_parametersMutex.unlock();

    if (!info.isValid()) {
        emit errorOccurred("Invalid device info");
        return nullptr;
    }

    m_controller = QLowEnergyController::createCentral(info);
    if (!m_controller) {
        qInfo() << "Failed to create central controller";
        return nullptr;
    }

    connect(m_controller, &QLowEnergyController::rssiRead, this, &BleCentral::rssiRead);
    connect(m_controller, &QLowEnergyController::disconnected, this, [this]() {
        emit errorOccurred("");
    });
    connect(m_controller, &QLowEnergyController::discoveryFinished, m_controller, [this]() {
        this->onDiscoveryFinished();
    });
    connect(m_controller, &QLowEnergyController::connected, this, [this]() {
        m_controller->discoverServices();
    });
    connect(m_controller, &QLowEnergyController::errorOccurred, m_controller, [this]() {
        emit errorOccurred(m_controller->errorString());
    });
    connect(m_controller,
            &QLowEnergyController::serviceDiscovered,
            m_controller,
            [this](const QBluetoothUuid &newService) {
                auto tmp = m_controller->createServiceObject(newService);
                if (tmp) {
                    qInfo() << "New service:" << tmp->serviceName() << newService;
                    tmp->deleteLater();
                } else {
                    qInfo() << "New service:" << newService;
                }
            });

    m_controller->connectToDevice();
    return m_controller;
}

void BleCentral::deinitDevice()
{
    m_controller->disconnectFromDevice();
    m_controller->deleteLater();
    m_controller = nullptr;
}

void BleCentral::writeBytesToDevice(const QByteArray &bytes)
{
    m_parametersMutex.lock();
    auto service = m_parameters["service"].value<QLowEnergyService *>();
    auto writeMode = m_parameters["writeMode"].value<QLowEnergyService::WriteMode>();
    auto characteristic = m_parameters["characteristic"].value<QLowEnergyCharacteristic>();
    m_parametersMutex.unlock();

    if (!characteristic.isValid()) {
        qInfo() << "Invalid characteristic";
        return;
    }

    if (!service) {
        qInfo() << "Failed to create service object";
        return;
    }

    if (writeMode == QLowEnergyService::WriteWithResponse) {
        if (characteristic.properties().testFlag(QLowEnergyCharacteristic::Write)) {
            service->writeCharacteristic(characteristic, bytes, writeMode);
        } else {
            emit warningOccurred(tr("Characteristic does not support write"));
        }
    } else {
        if (characteristic.properties().testFlag(QLowEnergyCharacteristic::WriteNoResponse)) {
            service->writeCharacteristic(characteristic, bytes, writeMode);
        } else {
            emit warningOccurred(tr("Characteristic does not support write without response"));
        }
    }
}

QList<QBluetoothUuid> BleCentral::services() const
{
    if (!m_controller) {
        return {};
    }

    return m_controller->services();
}

QString BleCentral::serviceName(const QBluetoothUuid &uuid) const
{
    if (!m_controller) {
        return {};
    }

    QLowEnergyService *service = m_controller->createServiceObject(uuid);
    if (!service) {
        return {};
    }

    QString name = service->serviceName();
    service->deleteLater();
    return name;
}

void BleCentral::setupService(QLowEnergyService *service)
{
    connect(service,
            &QLowEnergyService::characteristicChanged,
            service,
            [=](const QLowEnergyCharacteristic &info, const QByteArray &value) {
                emit bytesRead(value, info.name());
            });
    connect(service,
            &QLowEnergyService::characteristicRead,
            service,
            [=](const QLowEnergyCharacteristic &info, const QByteArray &value) {
                emit bytesRead(value, info.name());
            });
    connect(service,
            &QLowEnergyService::characteristicWritten,
            service,
            [=](const QLowEnergyCharacteristic &info, const QByteArray &value) {
                emit bytesWritten(value, info.name());
            });

    typedef QLowEnergyService::ServiceState ServiceState;
    connect(service, &QLowEnergyService::stateChanged, this, [this, service](ServiceState newState) {
        if (newState == QLowEnergyService::RemoteServiceDiscovered) {
            emit serviceDiscovered(service);
        }
    });
}

void BleCentral::onDiscoveryFinished()
{
    emit discoveryFinished();
    QList<QBluetoothUuid> uuids = m_controller->services();
    for (const QBluetoothUuid &uuid : uuids) {
        QLowEnergyService *service = m_controller->createServiceObject(uuid);
        if (!service) {
            continue;
        }

        setupService(service);
        service->discoverDetails();
    }
}
