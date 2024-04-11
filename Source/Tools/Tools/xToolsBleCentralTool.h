/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QVariantList>

#include "xToolsCommunicationTool.h"

class xToolsBleCentralTool : public xToolsCommunicationTool
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(QVariant info READ info WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QVariantList services READ services NOTIFY servicesChanged)
    Q_PROPERTY(int serviceIndex READ serviceIndex WRITE setServiceIndex NOTIFY serviceIndexChanged)
    Q_PROPERTY(int characteristicIndex READ characteristicIndex WRITE setCharacteristicIndex NOTIFY characteristicIndexChanged)
    Q_PROPERTY(int writeModel READ writeModel WRITE setWriteModel NOTIFY writeModelChanged)
    // clang-format on
public:
    xToolsBleCentralTool(QObject *parent = nullptr);
    ~xToolsBleCentralTool();

public:
    Q_INVOKABLE QString serviceName(QVariant service);
    Q_INVOKABLE QVariantList characteristics(QVariant service);
    Q_INVOKABLE QString characteristicName(QVariant characteristic);
    Q_INVOKABLE void readCharacteristic();
    Q_INVOKABLE void changeNotify();
    Q_INVOKABLE bool hasFlag(QVariant characteristic, int flag);
    Q_INVOKABLE bool isNotified(QVariant characteristic);

signals:
    void descriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);
    void serviceDiscoveryStarted();
    void serviceDiscoveryFinished();

protected:
    bool initialize(QString &errStr) override;
    void writeBytes(const QByteArray &bytes) override;
    void readBytes();
    void uninitialize() override;

private:
    QLowEnergyController *m_bleCentral{nullptr};

private:
    void onServiceDiscovered(const QBluetoothUuid &newService);
    void onServiceDiscoveryFinished();
    void onBleCentralErrorOccuured(QLowEnergyController::Error err);
    void onBleCentralConnected();
    void onBleCentralDisconnected();
    void onServiceObjectStateChanged(QLowEnergyService *service,
                                     QLowEnergyService::ServiceState newState);

public:
    Q_INVOKABLE QVariant info();
    Q_INVOKABLE void setInfo(QVariant info);
    Q_INVOKABLE QVariantList services();
    Q_INVOKABLE int serviceIndex();
    Q_INVOKABLE void setServiceIndex(int index);
    Q_INVOKABLE int characteristicIndex();
    Q_INVOKABLE void setCharacteristicIndex(int index);
    Q_INVOKABLE int writeModel();
    Q_INVOKABLE void setWriteModel(int model);

signals:
    void infoChanged();
    void servicesChanged();
    void serviceIndexChanged();
    void characteristicsChanged();
    void characteristicIndexChanged();
    void writeModelChanged();

private:
    QBluetoothDeviceInfo m_bluetoothDeviceInfo;
    QVector<QLowEnergyService *> m_services;
    int m_serviceIndex{-1};
    int m_characteristicIndex{-1};
    int m_writeModel;
};
