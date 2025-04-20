/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "blecentral.h"

BleCentral::BleCentral(QObject *parent)
    : Device(parent)
{}

BleCentral::~BleCentral() {}

QObject *BleCentral::initDevice()
{
    auto tmp = save();
    QBluetoothDeviceInfo info = tmp["deviceInfo"].value<QBluetoothDeviceInfo>();

    if (!info.isValid()) {
        emit errorOccurred("Invalid device info");
        return nullptr;
    }

    m_controller = QLowEnergyController::createCentral(info);
    if (!m_controller) {
        qInfo() << "Failed to create central controller";
        return nullptr;
    }
    //TODO:qt6
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
    //TODO:qt6
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

void BleCentral::writeActually(const QByteArray &bytes)
{
    QVariantMap parameters = save();
    auto service = parameters["service"].value<QLowEnergyService *>();
    auto writeMode = parameters["writeMode"].value<QLowEnergyService::WriteMode>();
    auto characteristic = parameters["characteristic"].value<QLowEnergyCharacteristic>();

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

QString BleCentral::getServiceName(const QVariant &service) const
{
    auto cookedService = service.value<QLowEnergyService *>();
    if (!cookedService) {
        return QString{};
    }

    return cookedService->serviceName();
}

QVariantList BleCentral::getCharacteristics(const QVariant &service) const
{
    QVariantList result;
    auto cookedService = service.value<QLowEnergyService *>();
    if (!cookedService) {
        return result;
    }

    auto characteristics = cookedService->characteristics();
    for (auto &characteristic : characteristics) {
        result.append(QVariant::fromValue(characteristic));
    }
    return result;
}

QString BleCentral::getCharacteristicName(const QVariant &characteristic) const
{
    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return QString{"Invalid"};
    }

    QString name = cookedCharacteristic.name();
    name = name.isEmpty() ? "(unnamed)" : name;
    return name;
}

bool BleCentral::testWriteFlag(const QVariant &characteristic) const
{
    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return false;
    }

    auto properties = cookedCharacteristic.properties();
    return properties.testFlag(QLowEnergyCharacteristic::Write);
}

bool BleCentral::testWriteNoResponseFlag(const QVariant &characteristic) const
{
    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return false;
    }

    auto properties = cookedCharacteristic.properties();
    return properties.testFlag(QLowEnergyCharacteristic::WriteNoResponse);
}

bool BleCentral::testNotifyFlag(const QVariant &characteristic) const
{
    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return false;
    }

    auto properties = cookedCharacteristic.properties();
    return properties.testFlag(QLowEnergyCharacteristic::Notify);
}

bool BleCentral::testReadFlag(const QVariant &characteristic) const
{
    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return false;
    }

    auto properties = cookedCharacteristic.properties();
    return properties.testFlag(QLowEnergyCharacteristic::Read);
}

void BleCentral::readCharacteristic(const QVariant &service, const QVariant &characteristic)
{
    auto cookedService = service.value<QLowEnergyService *>();
    if (!cookedService) {
        return;
    }

    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return;
    }

    cookedService->readCharacteristic(cookedCharacteristic);
}

void BleCentral::switchNotify(const QVariant &service, const QVariant &characteristic)
{
    auto cookedService = service.value<QLowEnergyService *>();
    if (!cookedService) {
        return;
    }

    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return;
    }

    auto type = QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration;
    auto descriptor = cookedCharacteristic.descriptor(type);
    if (!descriptor.isValid()) {
        qWarning("Invalid descriptor");
        return;
    }

    QByteArray value = descriptor.value() == notifyValue() ? unnotifyValue() : notifyValue();
    cookedService->writeDescriptor(descriptor, value);
}

void BleCentral::readNotify(const QVariant &service, const QVariant &characteristic)
{
    auto cookedService = service.value<QLowEnergyService *>();
    if (!cookedService) {
        return;
    }

    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return;
    }

    auto type = QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration;
    auto descriptor = cookedCharacteristic.descriptor(type);
    if (!descriptor.isValid()) {
        qWarning("Invalid descriptor");
        return;
    }

    cookedService->readDescriptor(descriptor);
}

bool BleCentral::isNotify(const QVariant &service, const QVariant &characteristic)
{
    auto cookedService = service.value<QLowEnergyService *>();
    if (!cookedService) {
        return false;
    }

    auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
    if (!cookedCharacteristic.isValid()) {
        return false;
    }

    auto type = QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration;
    auto descriptor = cookedCharacteristic.descriptor(type);
    if (!descriptor.isValid()) {
        qWarning("Invalid descriptor");
        return false;
    }

    return descriptor.value() == notifyValue();
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

QByteArray BleCentral::notifyValue()
{
    return QByteArray::fromHex("0100");
}

QByteArray BleCentral::unnotifyValue()
{
    return QByteArray::fromHex("0000");
}

void BleCentral::onDiscoveryFinished()
{
    emit discoveryFinished();
    emit opened();
    QList<QBluetoothUuid> uuids = m_controller->services();
    for (QBluetoothUuid &uuid : uuids) {
        QLowEnergyService *service = m_controller->createServiceObject(uuid);
        if (!service) {
            continue;
        }

        setupService(service);
        service->discoverDetails();
    }
}
