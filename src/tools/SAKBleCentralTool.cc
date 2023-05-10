/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMetaObject>
#include <QBluetoothDeviceInfo>

#include "SAKBleCentralTool.hh"

SAKBleCentralTool::SAKBleCentralTool(QObject *parent)
    : SAKCommunicationTool("SAK.BleCentral", parent)
{

}


SAKBleCentralTool::~SAKBleCentralTool()
{

}

QVariantList SAKBleCentralTool::serviceCharacteristics(QVariant service)
{
    QVariantList list;
    if (service.canConvert<QLowEnergyService*>()) {
        return list;
    }

    auto cookedService = service.value<QLowEnergyService*>();
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
        auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
        return cookedCharacteristic.name();
    }

    return "Invalid";
}

QString SAKBleCentralTool::serviceName(QVariant service)
{
    if (service.canConvert<QLowEnergyService*>()) {
        auto cookedService = service.value<QLowEnergyService*>();
        return cookedService->serviceName();
    }

    return "Invalid";
}

void SAKBleCentralTool::readCharacteristic()
{
    if (!(mCurrentServiceIndex >= 0 && mCurrentServiceIndex < mServices.length())) {
        return;
    }

    auto service = mServices.at(mCurrentServiceIndex);
    auto cookedService = service.value<QLowEnergyService*>();
    auto characteristics = cookedService->characteristics();
    if (mCurrentCharacteristicIndex >= 0 && mCurrentCharacteristicIndex < characteristics.length()) {
        auto characteristic = characteristics.at(mCurrentCharacteristicIndex);
        cookedService->readCharacteristic(characteristic);
    }
}

void SAKBleCentralTool::changeNotify()
{
    if (!(mCurrentServiceIndex >= 0 && mCurrentServiceIndex < mServices.length())) {
        return;
    }

    auto service = mServices.at(mCurrentServiceIndex);
    auto cookedService = service.value<QLowEnergyService*>();
    auto characteristics = cookedService->characteristics();
    if (!(mCurrentCharacteristicIndex >= 0 && mCurrentCharacteristicIndex < characteristics.length())) {
        return;
    }

    auto characteristic = characteristics.at(mCurrentCharacteristicIndex);
    auto descriptors = characteristic.descriptors();
    for (auto &descriptor : descriptors) {
        auto value = descriptor.value();
        auto type = QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration;
        if (descriptor.type() != type) {
            continue;
        }

        if (value == QByteArray::fromHex("0100")) {
            cookedService->writeDescriptor(descriptor, QByteArray::fromHex("0000"));
        } else {
            cookedService->writeDescriptor(descriptor, QByteArray::fromHex("0100"));
        }
    }
}

bool SAKBleCentralTool::initialize()
{
    if (!mBleInfo.isValid()) {
        outputMessage(QtWarningMsg, "Invalid BLE information.");
        return false;
    }

    mServices.clear();
    auto info = mBleInfo.value<QBluetoothDeviceInfo>();
    mBleCentral = QLowEnergyController::createCentral(info);
    connect(mBleCentral, &QLowEnergyController::serviceDiscovered, mBleCentral, [=](const QBluetoothUuid &newService){
        onServiceDiscovered(newService);
    });
    connect(mBleCentral, &QLowEnergyController::discoveryFinished, mBleCentral, [=](){
        onServiceDiscoveryFinished();
    });
    connect(mBleCentral, &QLowEnergyController::errorOccurred, mBleCentral, [=](){
        onServiceDiscoveryFinished();
    });


    mIsTimerReading = false;
    return true;
}

void SAKBleCentralTool::readBytes()
{

}

void SAKBleCentralTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    Q_UNUSED(context);
    if (!(mCurrentServiceIndex >= 0 && mCurrentServiceIndex < mServices.length())) {
        return;
    }

    auto service = mServices.at(mCurrentServiceIndex);
    auto cookedService = service.value<QLowEnergyService*>();

    auto characteristics = cookedService->characteristics();
    auto characteristic = characteristics.at(mCurrentCharacteristicIndex);
    if (mWriteModel == 0) {
        cookedService->writeCharacteristic(characteristic, bytes);
    } else {
        cookedService->writeCharacteristic(characteristic, bytes, QLowEnergyService::WriteWithoutResponse);
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
    Q_UNUSED(newService);
}

void SAKBleCentralTool::onServiceDiscoveryFinished()
{
    QList<QBluetoothUuid> uuids = mBleCentral->services();
    for (auto &uuid : uuids) {
        auto service = mBleCentral->createServiceObject(uuid);
        if (!service) {
            continue;
        }

        connect(service, &QLowEnergyService::characteristicChanged, service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value){
            emit bytesOutputted(value, QVariant());
            Q_UNUSED(info);
        });
        connect(service, &QLowEnergyService::characteristicRead, service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value){
            emit bytesOutputted(value, QVariant());
            Q_UNUSED(info);
        });
        connect(service, &QLowEnergyService::characteristicWritten, service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value){
            emit bytesInputted(value, QVariant());
            Q_UNUSED(info);
        });
        connect(service, &QLowEnergyService::stateChanged, service,
                [=](QLowEnergyService::ServiceState newState){
                    onServiceObjectStateChanged(service, newState);
        });
        connect(service, &QLowEnergyService::descriptorWritten,
                this, &SAKBleCentralTool::descriptorWritten);

        mServices.append(QVariant::fromValue(service));
        service->discoverDetails();
    }
}

void SAKBleCentralTool::onBleCentralErrorOccuured(QLowEnergyController::Error err)
{
    Q_UNUSED(err);
    qInfo() << mBleCentral->errorString();
    exit();
}

void SAKBleCentralTool::onServiceObjectStateChanged(QLowEnergyService *service, QLowEnergyService::ServiceState newState)
{
    Q_UNUSED(service);
    auto state = QLowEnergyService::RemoteServiceDiscovered;
    if (newState == state) {
        qInfo() << "Remote service discovered:" << newState;
    }
}
