/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKBLUETOOTHCLIENTDEVICE_HH
#define SAKBLUETOOTHCLIENTDEVICE_HH

#include <QThread>
#include <QEventLoop>
#include <QBluetoothSocket>
#include <QLowEnergyController>

#include "SAKDebugPageDevice.hh"

class SAKBluetoothClientDebugPage;
class SAKBluetoothClientDeviceController;
class SAKBluetoothClientDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKBluetoothClientDevice(SAKBluetoothClientDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
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
    bool mSpecifyClientAddressAndPort;
    QString mServerHost;
    quint16 mServerPort;
    QLowEnergyController *mLowEnergyController;
    SAKBluetoothClientDebugPage *mDebugPage;
    SAKBluetoothClientDeviceController *mDeviceController;
    QBluetoothSocket *mBluetoothSocket;
signals:
    void clientInfoChange(QString info);
};

#endif
