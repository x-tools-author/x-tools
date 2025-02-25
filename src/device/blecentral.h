/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QLowEnergyController>
#include <QLowEnergyService>

#include "communication.h"

namespace xTools {

class BleCentral : public Communication
{
    Q_OBJECT
public:
    explicit BleCentral(QObject *parent = nullptr);
    ~BleCentral() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeBytes(const QByteArray &bytes) override;

    QList<QBluetoothUuid> services() const;
    QString serviceName(const QBluetoothUuid &uuid) const;

    Q_INVOKABLE QString getServiceName(const QVariant &service) const;
    Q_INVOKABLE QVariantList getCharacteristics(const QVariant &service) const;
    Q_INVOKABLE QString getCharacteristicName(const QVariant &characteristic) const;

    Q_INVOKABLE bool testWriteFlag(const QVariant &characteristic) const;
    Q_INVOKABLE bool testWriteNoResponseFlag(const QVariant &characteristic) const;
    Q_INVOKABLE bool testNotifyFlag(const QVariant &characteristic) const;
    Q_INVOKABLE bool testReadFlag(const QVariant &characteristic) const;

    Q_INVOKABLE void readCharacteristic(const QVariant &service, const QVariant &characteristic);
    Q_INVOKABLE void switchNotify(const QVariant &service, const QVariant &characteristic);
    Q_INVOKABLE void readNotify(const QVariant &service, const QVariant &characteristic);
    Q_INVOKABLE bool isNotify(const QVariant &service, const QVariant &characteristic);

signals:
    void rssiRead(qint16 rssi);
    void serviceDiscovered(QLowEnergyService *service);
    void discoveryFinished();

private:
    QLowEnergyController *m_controller{nullptr};

private:
    void setupService(QLowEnergyService *service);
    QByteArray notifyValue();
    QByteArray unnotifyValue();

    void onDiscoveryFinished();
};

} // namespace xTools
