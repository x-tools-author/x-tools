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

#define BLE_ERR_SIG QLowEnergyController::Error

SAKBleCentralTool::SAKBleCentralTool(QObject *parent)
    : SAKCommunicationTool("SAK.BleCentral", parent)
{

}


SAKBleCentralTool::~SAKBleCentralTool()
{

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
    for (auto &var : mBleServiceObjects) {
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

QString SAKBleCentralTool::serviceName(QVariant service)
{
    QObject *obj = service.value<QObject*>();
    auto cookedService = qobject_cast<QLowEnergyService*>(obj);
    if (cookedService) {
        return cookedService->serviceName();
    } else {
        outputMessage(QtWarningMsg, "invalid valud of service, not a object!");
    }

    return "Invalid";
}

QVariantList SAKBleCentralTool::characteristics(QVariant service)
{
    QVariantList list;
    if (service.canConvert<QLowEnergyService*>()) {
        return list;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    auto cookedService = service.value<QLowEnergyService*>();
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
    if (!(mServiceIndex >= 0
          && mServiceIndex < mServices.length())) {
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto cookedService = service.value<QLowEnergyService*>();
    auto characteristics = cookedService->characteristics();
    if ((mCharacteristicIndex >= 0)
        && (mCharacteristicIndex < characteristics.length())) {
        auto c = characteristics.at(mCharacteristicIndex);
        cookedService->readCharacteristic(c);
    }
}

void SAKBleCentralTool::changeNotify()
{
    if (!((mServiceIndex >= 0)
          && (mServiceIndex < mServices.length()))) {
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto cookedService = service.value<QLowEnergyService*>();
    auto characteristics = cookedService->characteristics();
    if (!((mCharacteristicIndex >= 0)
          && (mCharacteristicIndex < characteristics.length()))) {
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
            cookedService->writeDescriptor(descriptor, value);
        } else {
            QByteArray value = QByteArray::fromHex("0100");
            cookedService->writeDescriptor(descriptor, value);
        }
    }
}

bool SAKBleCentralTool::initialize(QString &errStr)
{
    if (!mBluetoothDeviceInfo.isValid()) {
        errStr = "invalid ble information.";
        outputMessage(QtWarningMsg, errStr);
        return false;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    mServices.clear();
    mBleCentral = QLowEnergyController::createCentral(mBluetoothDeviceInfo);
    connect(mBleCentral, &QLowEnergyController::serviceDiscovered, mBleCentral,
            [=](const QBluetoothUuid &newService){
        onServiceDiscovered(newService);
    });
    connect(mBleCentral, &QLowEnergyController::discoveryFinished,
            mBleCentral, [=](){
        onServiceDiscoveryFinished();
    });
    connect(mBleCentral,
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
            &QLowEnergyController::errorOccurred,
#else
            QOverload<BLE_ERR_SIG>::of(&QLowEnergyController::error),
#endif
            mBleCentral,
            [=](QLowEnergyController::Error err){
        onBleCentralErrorOccuured(err);
    });
    connect(mBleCentral, &QLowEnergyController::connected,
            mBleCentral, [=](){onBleCentralConnected();});
    connect(mBleCentral, &QLowEnergyController::disconnected,
            mBleCentral, [=](){onBleCentralDisconnected();});


    mBleCentral->connectToDevice();
    qDebug() << "123";
    return true;
#else
    return false;
#endif
}

void SAKBleCentralTool::readBytes()
{

}

void SAKBleCentralTool::writeBytes(const QByteArray &bytes,
                                   const QVariant &context)
{
    Q_UNUSED(context);
    if (!((mServiceIndex >= 0)
          && (mServiceIndex < mServices.length()))) {
        return;
    }

    auto service = mServices.at(mServiceIndex);
    auto cookedService = service.value<QLowEnergyService*>();

    auto characteristics = cookedService->characteristics();
    auto characteristic = characteristics.at(mCharacteristicIndex);
    if (mWriteModel == 0) {
        cookedService->writeCharacteristic(characteristic, bytes);
    } else {
        auto opt = QLowEnergyService::WriteWithoutResponse;
        cookedService->writeCharacteristic(characteristic, bytes, opt);
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
    outputMessage(QtInfoMsg, "new ble service discovered:"
                                 + newService.toString());
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
                [=](const QLowEnergyCharacteristic &info,
                    const QByteArray &value){
            emit bytesOutputted(value, QVariant());
            Q_UNUSED(info);
        });
        connect(service, &QLowEnergyService::characteristicRead, service,
                [=](const QLowEnergyCharacteristic &info,
                    const QByteArray &value){
            emit bytesOutputted(value, QVariant());
            Q_UNUSED(info);
        });
        connect(service, &QLowEnergyService::characteristicWritten, service,
                [=](const QLowEnergyCharacteristic &info,
                    const QByteArray &value){
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

void SAKBleCentralTool::onBleCentralErrorOccuured(
    QLowEnergyController::Error err)
{
    Q_UNUSED(err)
    outputMessage(QtInfoMsg, "new ble service error:"
                                 + mBleCentral->errorString());
    exit();
}

void SAKBleCentralTool::onBleCentralConnected()
{
    mBleCentral->discoverServices();
}

void SAKBleCentralTool::onBleCentralDisconnected()
{

}

void SAKBleCentralTool::onServiceObjectStateChanged(
    QLowEnergyService *service, QLowEnergyService::ServiceState newState)
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
