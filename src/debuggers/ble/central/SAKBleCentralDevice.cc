/******************************************************************************
 * Copyright 2022 wuhai(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QDebug>
#include <QElapsedTimer>
#include <QBluetoothDeviceInfo>

#include "SAKBleCentralDevice.hh"
#include "SAKCommonDataStructure.hh"

SAKBleCentralDevice::SAKBleCentralDevice(QSettings *settings,
                                         const QString &settingsGroup,
                                         QWidget *uiParent,
                                         QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mBleController(Q_NULLPTR)
{

}

bool SAKBleCentralDevice::initialize()
{
    QVariant rawCtx = parametersContext();
    auto parasCtx = rawCtx.value<SAKBleCentralParametersContext>();
    QBluetoothDeviceInfo info = parasCtx.info;

    mBleController = QLowEnergyController::createCentral(info);
    if (!mBleController) {
        qDebug() << "Create central failed!";
        return false;
    }

    connect(mBleController, &QLowEnergyController::connected,
            mBleController, [=](){
        qDebug() << "Connect to device successfully!";
        mBleController->discoverServices();
    });

    connect(mBleController,
            QOverload<QLowEnergyController::Error>::of(
                &QLowEnergyController::error),
            mBleController, [=](QLowEnergyController::Error err){
        qDebug() << err << mBleController->errorString();
        return false;
    });

    connect(mBleController,  &QLowEnergyController::serviceDiscovered,
            mBleController, [=](const QBluetoothUuid &newService){
        auto *service = mBleController->createServiceObject(
                    newService, mBleController);
        if (!service) {
            qWarning() << "Create service object failed!";
            return;
        }

        connect(service, &QLowEnergyService::descriptorRead,
                service,
                [](const QLowEnergyDescriptor &info, const QByteArray &array){
            qDebug() << "descriptorRead:" << QString(array.toHex(' '));
        });
        connect(service, &QLowEnergyService::descriptorWritten,
                service,
                [](const QLowEnergyDescriptor &info, const QByteArray &array){
            qDebug() << "descriptorWritten:" << QString(array.toHex(' '));
        });
        connect(service, &QLowEnergyService::characteristicRead,
                service,
                [](const QLowEnergyCharacteristic &characteristic,
                const QByteArray &array){
            qDebug() << "characteristicRead:" << QString(array.toHex(' '));
        });
        connect(service, &QLowEnergyService::characteristicWritten,
                service,
                [](const QLowEnergyCharacteristic &characteristic,
                const QByteArray &array){
            qDebug() << "characteristicWritten:" << QString(array.toHex(' '));
        });
        connect(service, &QLowEnergyService::stateChanged,
                service,
                [](QLowEnergyService::ServiceState newState){
            qDebug() << "stateChanged:" << newState;
        });
        connect(service,
                QOverload<QLowEnergyService::ServiceError>::of(
                    &QLowEnergyService::error),
                service,
                [](QLowEnergyService::ServiceError error){
            qDebug() << "error:" << error;
        });

        service->discoverDetails();
    });

    connect(mBleController, &QLowEnergyController::discoveryFinished,
            mBleController, [=](){
        qDebug() << "discoveryFinished";
        auto uuids = mBleController->services();
        QStringList services;
        for (QBluetoothUuid &uuid : uuids) {
            services.append(uuid.toString());
        }
        emit servicesChanged(services);
    });

    connect(mBleController, &QLowEnergyController::disconnected,
            mBleController, [=](){
        qDebug() << "disconnected:" << mBleController->errorString();
        exit();
    });

    mBleController->connectToDevice();
    return true;
}

SAKDebuggerDevice::ReadContextVector SAKBleCentralDevice::read()
{
    auto parasCtx = parametersContext().value<SAKBleCentralParametersContext>();
    QBluetoothUuid uuid(parasCtx.uuid);
//    const int defauleFrameInterval = 4;
//    int frameIntervel = parasCtx.frameIntervel;
//    qint64 frameIntervelNs = 0;
//    qint64 consumeNsPerBit = (1000*1000*1000)/parasCtx.baudRate;
//    if (frameIntervel < defauleFrameInterval) {
//        frameIntervelNs = defauleFrameInterval*consumeNsPerBit;
//    } else {
//        frameIntervelNs = frameIntervel*consumeNsPerBit;
//    }

//    QElapsedTimer elapsedTimer;
//    QByteArray bytes;
//    while (true) {
//        if (!mSerialPort) {
//            break;
//        }

//        auto ret = mSerialPort->readAll();
//        if (ret.length()) {
//            bytes.append(ret);
//            elapsedTimer.restart();
//        } else {
//            auto ret = elapsedTimer.nsecsElapsed();
//            if (frameIntervelNs <= ret) {
//                break;
//            }
//        }
//    }
    ReadContextVector contexts;

//    if (bytes.length()) {
//        if (mSerialPort) {
//            ReadContext ctx;
//            ctx.bytes = bytes;
//            ctx.flag = mSerialPort->portName();
//            return contexts << ctx;
//        }

//    }

    return contexts;
}

SAKDebuggerDevice::WriteContext SAKBleCentralDevice::write(const QByteArray &bytes)
{
    WriteContext context;
//    context.flag = mSerialPort->portName();
//    if (mSerialPort->write(bytes) > 0) {
//        context.bytes = bytes;
//    } else {
//        emit errorOccurred(mSerialPort->errorString());
//        qWarning() << "Write bytes failed:" << mSerialPort->errorString();
//    }
    return context;
}

void SAKBleCentralDevice::uninitialize()
{
    if (mBleController) {
        mBleController->disconnectFromDevice();
    }

    mBleController = Q_NULLPTR;
}
