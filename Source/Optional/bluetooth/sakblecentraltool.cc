/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QBluetoothDeviceInfo>
#include <QMetaObject>

#include "sakblecentraltool.h"

#define BLE_ERR_SIG QLowEnergyController::Error

SAKBleCentralTool::SAKBleCentralTool(QObject *parent)
    : SAKCommunicationTool(parent)
{}

SAKBleCentralTool::~SAKBleCentralTool() {}

QString SAKBleCentralTool::serviceName(QVariant service)
{
    QObject *obj = service.value<QObject *>();
    auto cookedService = qobject_cast<QLowEnergyService *>(obj);
    if (cookedService) {
        return cookedService->serviceName();
    } else {
        qWarning() << "invalid valud of service, not a object!";
    }

    return "Invalid";
}

QVariantList SAKBleCentralTool::characteristics(QVariant service)
{
    QVariantList list;
    if (service.canConvert<QLowEnergyService *>()) {
        return list;
    }
    auto cookedService = service.value<QLowEnergyService *>();
    if (cookedService) {
        auto characteristics = cookedService->characteristics();
        for (auto &characteristic : characteristics) {
            list.append(QVariant::fromValue(characteristic));
        }
    }
    return list;
}

QString SAKBleCentralTool::characteristicName(QVariant characteristic)
{
    if (characteristic.canConvert<QLowEnergyCharacteristic>()) {
        auto c = characteristic.value<QLowEnergyCharacteristic>();
        return c.name();
    }

    return "Invalid";
}

void SAKBleCentralTool::readCharacteristic()
{
    if (!(m_serviceIndex >= 0 && m_serviceIndex < m_services.length())) {
        qWarning() << "invalid service index";
        return;
    }

    auto service = m_services.at(m_serviceIndex);
    auto chs = service->characteristics();
    if (!(m_characteristicIndex >= 0 && m_characteristicIndex < chs.count())) {
        qWarning() << "invalid characteristic index";
        return;
    }

    auto c = chs.at(m_characteristicIndex);
    service->readCharacteristic(c);
}

void SAKBleCentralTool::changeNotify()
{
    if (!((m_serviceIndex >= 0) && (m_serviceIndex < m_services.length()))) {
        return;
    }

    auto service = m_services.at(m_serviceIndex);
    auto characteristics = service->characteristics();
    if (!((m_characteristicIndex >= 0) && (m_characteristicIndex < characteristics.length()))) {
        return;
    }

    auto characteristic = characteristics.at(m_characteristicIndex);
    auto descriptors = characteristic.descriptors();
    typedef QBluetoothUuid::DescriptorType SAKDescriptorType;
    auto type = SAKDescriptorType::ClientCharacteristicConfiguration;
    for (auto &descriptor : descriptors) {
        auto value = descriptor.value();
        if (descriptor.type() != type) {
            continue;
        }

        if (value == QByteArray::fromHex("0100")) {
            QByteArray value = QByteArray::fromHex("0000");
            service->writeDescriptor(descriptor, value);
            qWarning() << "disable notify";
        } else {
            QByteArray value = QByteArray::fromHex("0100");
            service->writeDescriptor(descriptor, value);
            qWarning() << "enable notify";
        }
    }
}

bool SAKBleCentralTool::hasFlag(QVariant characteristic, int flag)
{
    auto ch = characteristic.value<QLowEnergyCharacteristic>();
    auto properties = ch.properties();
    if (properties & flag) {
        return true;
    }

    return false;
}

bool SAKBleCentralTool::isNotified(QVariant characteristic)
{
    auto cookedCh = characteristic.value<QLowEnergyCharacteristic>();
    auto desList = cookedCh.descriptors();
    bool notified = false;
    for (auto &des : desList) {
        if (des.value() == QByteArray::fromHex("0100")) {
            notified = true;
            break;
        }
    }

    return notified;
}

bool SAKBleCentralTool::initialize(QString &errStr)
{
    if (!m_bluetoothDeviceInfo.isValid()) {
        errStr = "invalid ble information.";
        return false;
    }

    m_services.clear();
    mBleCentral = QLowEnergyController::createCentral(m_bluetoothDeviceInfo);
    connect(mBleCentral,
            &QLowEnergyController::serviceDiscovered,
            mBleCentral,
            [=](const QBluetoothUuid &newService) { onServiceDiscovered(newService); });
    connect(mBleCentral, &QLowEnergyController::discoveryFinished, mBleCentral, [=]() {
        onServiceDiscoveryFinished();
    });
    connect(mBleCentral,
            &QLowEnergyController::errorOccurred,
            mBleCentral,
            [=](QLowEnergyController::Error err) { onBleCentralErrorOccuured(err); });
    connect(mBleCentral, &QLowEnergyController::connected, mBleCentral, [=]() {
        onBleCentralConnected();
    });
    connect(mBleCentral, &QLowEnergyController::disconnected, mBleCentral, [=]() {
        onBleCentralDisconnected();
    });

    mBleCentral->connectToDevice();
    emit serviceDiscoveryStarted();
    return true;
}

void SAKBleCentralTool::readBytes()
{
    if (!((m_serviceIndex >= 0) && (m_serviceIndex < m_services.length()))) {
        return;
    }

    auto service = m_services.at(m_serviceIndex);
    auto characteristics = service->characteristics();
    auto characteristic = characteristics.at(m_characteristicIndex);
    service->readCharacteristic(characteristic);
}

void SAKBleCentralTool::writeBytes(const QByteArray &bytes)
{
    if (!((m_serviceIndex >= 0) && (m_serviceIndex < m_services.length()))) {
        qWarning() << "invalid parameters.";
        return;
    }

    auto service = m_services.at(m_serviceIndex);
    auto characteristics = service->characteristics();
    auto characteristic = characteristics.at(m_characteristicIndex);
    if (m_writeModel == 0) {
        if (!hasFlag(QVariant::fromValue(characteristic), QLowEnergyCharacteristic::Write)) {
            QString str = "QLowEnergyService::WriteWithResponse";
            qWarning() << "unsupported write model: " + str;
            return;
        }

        qInfo() << "try to write bytes:" << QString::fromLatin1(bytes.toHex());
        service->writeCharacteristic(characteristic, bytes);
    } else {
        if (!hasFlag(QVariant::fromValue(characteristic),
                     QLowEnergyCharacteristic::WriteNoResponse)) {
            QString str = "QLowEnergyService::WriteWithoutResponse";
            qWarning() << "unsupported write model: " + str;
            return;
        }

        qInfo() << "try to write bytes without response:"
                                 << QString::fromLatin1(bytes.toHex());
        auto opt = QLowEnergyService::WriteWithoutResponse;
        service->writeCharacteristic(characteristic, bytes, opt);
    }
}

void SAKBleCentralTool::uninitialize()
{
    mBleCentral->disconnectFromDevice();
    mBleCentral->deleteLater();
    mBleCentral = nullptr;
}

void SAKBleCentralTool::onServiceDiscovered(const QBluetoothUuid &newService)
{
    qInfo() << "new ble service discovered:" + newService.toString();
}

void SAKBleCentralTool::onServiceDiscoveryFinished()
{
    qInfo() << "ble service discovery finished.";
    QList<QBluetoothUuid> uuids = mBleCentral->services();
    qInfo() << "service count:" + QString::number(uuids.length());

    for (auto &uuid : uuids) {
        auto service = mBleCentral->createServiceObject(uuid);
        if (!service) {
            continue;
        }

        connect(service,
                &QLowEnergyService::characteristicChanged,
                service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value) {
                    emit outputBytes(value);
                    emit bytesRead(value, info.name());
                });
        connect(service,
                &QLowEnergyService::characteristicRead,
                service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value) {
                    emit outputBytes(value);
                    emit bytesRead(value, info.name());
                });
        connect(service,
                &QLowEnergyService::characteristicWritten,
                service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value) {
                    emit bytesWritten(value, info.name());
                });
        connect(service,
                &QLowEnergyService::stateChanged,
                service,
                [=](QLowEnergyService::ServiceState newState) {
                    onServiceObjectStateChanged(service, newState);
                });
        connect(service,
                &QLowEnergyService::descriptorWritten,
                this,
                &SAKBleCentralTool::descriptorWritten);

        m_services.append(service);
        service->discoverDetails();
    }

    emit serviceDiscoveryFinished();
}

void SAKBleCentralTool::onBleCentralErrorOccuured(QLowEnergyController::Error err)
{
    if (err == QLowEnergyController::UnknownError) {
        return;
    }

    qWarning() << "new ble service error:" + mBleCentral->errorString();
    exit();
}

void SAKBleCentralTool::onBleCentralConnected()
{
    qWarning() << "connect to device successfully.";
    mBleCentral->discoverServices();
}

void SAKBleCentralTool::onBleCentralDisconnected()
{
    QString msg = "disconnect from device";
    qWarning() << msg;
    emit errorOccurred(msg);
}

void SAKBleCentralTool::onServiceObjectStateChanged(QLowEnergyService *service,
                                                    QLowEnergyService::ServiceState newState)
{
    Q_UNUSED(service);
    auto state = QLowEnergyService::RemoteServiceDiscovered;
    if (newState == state) {
        qInfo() << "Remote service discovered:" << newState;
    }
}

QVariant SAKBleCentralTool::info()
{
    return QVariant::fromValue(m_bluetoothDeviceInfo);
}

void SAKBleCentralTool::setInfo(QVariant info)
{
    m_bluetoothDeviceInfo = info.value<QBluetoothDeviceInfo>();
    emit infoChanged();
}

QVariantList SAKBleCentralTool::services()
{
    QVariantList varList;
    for (auto &var : m_services) {
        varList.append(QVariant::fromValue(var));
    }
    return varList;
}

int SAKBleCentralTool::serviceIndex()
{
    return m_serviceIndex;
}

void SAKBleCentralTool::setServiceIndex(int index)
{
    m_serviceIndex = index;
    emit serviceIndexChanged();
}

int SAKBleCentralTool::characteristicIndex()
{
    return m_characteristicIndex;
}

void SAKBleCentralTool::setCharacteristicIndex(int index)
{
    m_characteristicIndex = index;
    emit characteristicIndexChanged();
}

int SAKBleCentralTool::writeModel()
{
    return m_writeModel;
}

void SAKBleCentralTool::setWriteModel(int model)
{
    m_writeModel = model;
    emit writeModelChanged();
}
