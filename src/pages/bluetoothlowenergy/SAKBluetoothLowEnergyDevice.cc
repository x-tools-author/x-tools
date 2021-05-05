/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>
#include <QBluetoothAddress>

#include "SAKBluetoothLowEnergyDevice.hh"
#include "SAKBluetoothLowEnergyDebugPage.hh"
#include "SAKBluetoothLowEnergyDeviceController.hh"

SAKBluetoothLowEnergyDevice::SAKBluetoothLowEnergyDevice(SAKBluetoothLowEnergyDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
{

}

bool SAKBluetoothLowEnergyDevice::initializing(QString &errorString)
{
    QEventLoop eventLoop;
    mDeviceController = qobject_cast<SAKBluetoothLowEnergyDeviceController*>(mDebugPage->deviceController());
    auto parameters = mDeviceController->parameters().value<SAKBluetoothLowEnergyDeviceController::BluetoothLowEnergyParameters>();
    auto info = parameters.deviceInfo;

    mLowEnergyController = new QLowEnergyController(info);
    connect(mLowEnergyController, &QLowEnergyController::serviceDiscovered, this, [](QBluetoothUuid serviceUuid){
        qDebug() << serviceUuid;
    }, Qt::DirectConnection);

    Q_UNUSED(errorString);
    return true;
}

bool SAKBluetoothLowEnergyDevice::open(QString &errorString)
{
    mLowEnergyController->connectToDevice();
    sleep(3);
    if (!(mLowEnergyController->state() == QLowEnergyController::ConnectedState)){
        return false;
    } else {
        emit messageChanged("Have been connected to device.", true);
    }

//    mLowEnergyController->discoverServices();
//    connect(mLowEnergyController, &QLowEnergyController::serviceDiscovered, this, [this](QBluetoothUuid serviceUuid){
//        qDebug() << serviceUuid;
//        auto service = mLowEnergyController->createServiceObject(serviceUuid);
//        connect(service, &QLowEnergyService::stateChanged, this, [=](){
//            qDebug() << "service state change" << mLowEnergyController->state() << ",||||||";
//            //发现服务, 建立characteristic对象实例
//            if(service->state() == QLowEnergyService::ServiceDiscovered) {
//                QLowEnergyCharacteristic hrChar = service->characteristic(QBluetoothUuid(quint16(0xfff6)));
//                if(!hrChar.isValid()) {
//                    qDebug() << "characteristic fff6 error:::";
//                }
//                // 设置特征对象可用
//                //enable the chracteristic notification by write 0x01 to LowEnergy characteristic configuration
//                QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::LowEnergyCharacteristicConfiguration);
//                if (m_notificationDesc.isValid()) {
//                    if(hrChar.properties() & QLowEnergyCharacteristic::Notify) {
//                        qDebug() << "123";
//                    }
//                    service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
//                }
//            }

//            connect(service, &QLowEnergyService::characteristicChanged, this, [](QLowEnergyCharacteristic characteristic, QByteArray value){
//                Q_UNUSED(characteristic);
//                qDebug() << value;
//            });
//        });
//    });
    Q_UNUSED(errorString);
    return mLowEnergyController->state() == QLowEnergyController::ConnectedState;
}

QByteArray SAKBluetoothLowEnergyDevice::read()
{
//    return mTcpSocket->readAll();
    return QByteArray();
}

QByteArray SAKBluetoothLowEnergyDevice::write(QByteArray bytes)
{
//    qint64 ret = mTcpSocket->write(bytes);
//    if (ret > 0){
//        return bytes;
//    }else{
//        return QByteArray();
//    }
    return QByteArray();
}

bool SAKBluetoothLowEnergyDevice::checkSomething(QString &errorString)
{
//    if(mTcpSocket->state() == QTcpSocket::UnconnectedState){
//        errorString = tr("Connection has been disconnected.");
//        return false;
//    }else{
//        errorString = tr("Unknown error");
//        return true;
//    }
    return true;
}

void SAKBluetoothLowEnergyDevice::close()
{
    emit messageChanged("Have been disconnected to device.", true);
    mLowEnergyController->disconnectFromDevice();
}

void SAKBluetoothLowEnergyDevice::free()
{
    delete mLowEnergyController;
    mLowEnergyController = Q_NULLPTR;
}
