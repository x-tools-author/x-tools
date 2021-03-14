/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKBLUETOOTHLOWENERGYDEVICE_HH
#define SAKBLUETOOTHLOWENERGYDEVICE_HH

#include <QThread>
#include <QEventLoop>
#include <QBluetoothSocket>
#include <QLowEnergyController>

#include "SAKDebugPageDevice.hh"

class SAKBluetoothLowEnergyDebugPage;
class SAKBluetoothLowEnergyDeviceController;
class SAKBluetoothLowEnergyDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKBluetoothLowEnergyDevice(SAKBluetoothLowEnergyDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
private:
    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    bool checkSomething(QString &errorString) final;
    void close() final;
    void free() final;
private:
    QString mLocalHost;
    quint16 mLocalPort;
    bool mSpecifyLowEnergyAddressAndPort;
    QString mServerHost;
    quint16 mServerPort;
    QLowEnergyController *mLowEnergyController;
    SAKBluetoothLowEnergyDebugPage *mDebugPage;
    SAKBluetoothLowEnergyDeviceController *mDeviceController;
    QBluetoothSocket *mBluetoothSocket;
signals:
    void LowEnergyInfoChange(QString info);
};

#endif
