/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    auto cookedService = service.value<QLowEnergyService *>();
    if (cookedService) {
        auto characteristics = cookedService->characteristics();
        for (auto &characteristic : characteristics) {
            list.append(QVariant::fromValue(characteristic));
        }
    }
#endif
    return list;
}

QString SAKBleCentralTool::characteristicName(QVariant characteristic)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    if (characteristic.canConvert<QLowEnergyCharacteristic>()) {
        auto c = characteristic.value<QLowEnergyCharacteristic>();
        return c.name();
    }
#else
    Q_UNUSED(characteristic)
#endif

    return "Invalid";
}

void SAKBleCentralTool::readCharacteristic()
{
    if (!(mServiceIndex >= 0 && mServiceIndex < mServices.length())) {
        qWarning() << "invalid service index";
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto chs = service->characteristics();
    if (!(mCharacteristicIndex >= 0 && mCharacteristicIndex < chs.count())) {
        qWarning() << "invalid characteristic index";
        return;
    }

    auto c = chs.at(mCharacteristicIndex);
    service->readCharacteristic(c);
}

void SAKBleCentralTool::changeNotify()
{
    if (!((mServiceIndex >= 0) && (mServiceIndex < mServices.length()))) {
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto characteristics = service->characteristics();
    if (!((mCharacteristicIndex >= 0) && (mCharacteristicIndex < characteristics.length()))) {
        return;
    }

    auto characteristic = characteristics.at(mCharacteristicIndex);
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
    if (!mBluetoothDeviceInfo.isValid()) {
        errStr = "invalid ble information.";
        return false;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    mServices.clear();
    mBleCentral = QLowEnergyController::createCentral(mBluetoothDeviceInfo);
    connect(mBleCentral,
            &QLowEnergyController::serviceDiscovered,
            mBleCentral,
            [=](const QBluetoothUuid &newService) { onServiceDiscovered(newService); });
    connect(mBleCentral, &QLowEnergyController::discoveryFinished, mBleCentral, [=]() {
        onServiceDiscoveryFinished();
    });
    connect(mBleCentral,
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
            &QLowEnergyController::errorOccurred,
#else
            QOverload<BLE_ERR_SIG>::of(&QLowEnergyController::error),
#endif
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
#else
    return false;
#endif
}

void SAKBleCentralTool::readBytes()
{
    if (!((mServiceIndex >= 0) && (mServiceIndex < mServices.length()))) {
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto characteristics = service->characteristics();
    auto characteristic = characteristics.at(mCharacteristicIndex);
    service->readCharacteristic(characteristic);
}

void SAKBleCentralTool::writeBytes(const QByteArray &bytes)
{
    if (!((mServiceIndex >= 0) && (mServiceIndex < mServices.length()))) {
        qWarning() << "invalid parameters.";
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto characteristics = service->characteristics();
    auto characteristic = characteristics.at(mCharacteristicIndex);
    if (mWriteModel == 0) {
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

        mServices.append(service);
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
    emit errorOccured(msg);
}

void SAKBleCentralTool::onServiceObjectStateChanged(QLowEnergyService *service,
                                                    QLowEnergyService::ServiceState newState)
{
    Q_UNUSED(service);
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
    auto state = QLowEnergyService::RemoteServiceDiscovered;
#else
    auto state = QLowEnergyService::ServiceDiscovered;
#endif
    if (newState == state) {
        qInfo() << "Remote service discovered:" << newState;
    }
}

QVariant SAKBleCentralTool::info()
{
    return QVariant::fromValue(mBluetoothDeviceInfo);
}

void SAKBleCentralTool::setInfo(QVariant info)
{
    mBluetoothDeviceInfo = info.value<QBluetoothDeviceInfo>();
    emit infoChanged();
}

QVariantList SAKBleCentralTool::services()
{
    QVariantList varList;
    for (auto &var : mServices) {
        varList.append(QVariant::fromValue(var));
    }
    return varList;
}

int SAKBleCentralTool::serviceIndex()
{
    return mServiceIndex;
}

void SAKBleCentralTool::setServiceIndex(int index)
{
    mServiceIndex = index;
    emit serviceIndexChanged();
}

int SAKBleCentralTool::characteristicIndex()
{
    return mCharacteristicIndex;
}

void SAKBleCentralTool::setCharacteristicIndex(int index)
{
    mCharacteristicIndex = index;
    emit characteristicIndexChanged();
}

int SAKBleCentralTool::writeModel()
{
    return mWriteModel;
}

void SAKBleCentralTool::setWriteModel(int model)
{
    mWriteModel = model;
    emit writeModelChanged();
}
