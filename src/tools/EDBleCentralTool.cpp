/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QMetaObject>
#include <QBluetoothDeviceInfo>

#include "EDBleCentralTool.hpp"

EDBleCentralTool::EDBleCentralTool(QObject *parent)
    : EDBaseTool("ED.BleCentral", parent)
{

}


EDBleCentralTool::~EDBleCentralTool()
{

}

QVariantList EDBleCentralTool::serviceCharacteristics(QVariant service)
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

QString EDBleCentralTool::characteristicName(QVariant characteristic)
{
    if (characteristic.canConvert<QLowEnergyCharacteristic>()) {
        auto cookedCharacteristic = characteristic.value<QLowEnergyCharacteristic>();
        return cookedCharacteristic.name();
    }

    return "Invalid";
}

QString EDBleCentralTool::serviceName(QVariant service)
{
    if (service.canConvert<QLowEnergyService*>()) {
        auto cookedService = service.value<QLowEnergyService*>();
        return cookedService->serviceName();
    }

    return "Invalid";
}

void EDBleCentralTool::readCharacteristic()
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

void EDBleCentralTool::changeNotify()
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

bool EDBleCentralTool::initialize(QString &errStr)
{
    if (!mBleInfo.isValid()) {
        errStr = "Invalid BLE information.";
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


    return true;
}

void EDBleCentralTool::inputBytesHandler(const QByteArray &bytes)
{
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

void EDBleCentralTool::uninitialize()
{
    mBleCentral->disconnectFromDevice();
    mBleCentral->deleteLater();
    mBleCentral = nullptr;
}

void EDBleCentralTool::onServiceDiscovered(const QBluetoothUuid &newService)
{
    Q_UNUSED(newService);
}

void EDBleCentralTool::onServiceDiscoveryFinished()
{
    QList<QBluetoothUuid> uuids = mBleCentral->services();
    for (auto &uuid : uuids) {
        auto service = mBleCentral->createServiceObject(uuid);
        if (!service) {
            continue;
        }

        connect(service, &QLowEnergyService::characteristicChanged, service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value){
                    emit bytesOutputted(info.name(), value);
        });
        connect(service, &QLowEnergyService::characteristicRead, service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value){
                    emit bytesOutputted(info.name(), value);
        });
        connect(service, &QLowEnergyService::characteristicWritten, service,
                [=](const QLowEnergyCharacteristic &info, const QByteArray &value){
                    emit bytesInputted(info.name(), value);
        });
        connect(service, &QLowEnergyService::stateChanged, service,
                [=](QLowEnergyService::ServiceState newState){
                    onServiceObjectStateChanged(service, newState);
        });
        connect(service, &QLowEnergyService::descriptorWritten,
                this, &EDBleCentralTool::descriptorWritten);

        mServices.append(QVariant::fromValue(service));
        service->discoverDetails();
    }
}

void EDBleCentralTool::onBleCentralErrorOccuured(QLowEnergyController::Error err)
{
    Q_UNUSED(err);
    qInfo() << mBleCentral->errorString();
    exit();
}

void EDBleCentralTool::onServiceObjectStateChanged(QLowEnergyService *service, QLowEnergyService::ServiceState newState)
{
    auto state = QLowEnergyService::RemoteServiceDiscovered;
    if (newState == state) {
        qInfo() << "Remote service discovered:" << newState;
    }
}
