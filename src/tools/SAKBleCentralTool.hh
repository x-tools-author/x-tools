/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef EDBLECENTRAL_HH
#define EDBLECENTRAL_HH

#include <QVariantList>
#include <QBluetoothUuid>
#include <QLowEnergyService>
#include <QLowEnergyController>
#include <QBluetoothDeviceInfo>

#include "SAKCommunicationTool.hh"

class SAKBleCentralTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QVariant info READ info WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QVariantList services READ services NOTIFY servicesChanged)
    Q_PROPERTY(int serviceIndex READ serviceIndex WRITE setServiceIndex
               NOTIFY serviceIndexChanged)
    Q_PROPERTY(int characteristicIndex READ characteristicIndex
               WRITE setCharacteristicIndex
               NOTIFY characteristicIndexChanged)
    Q_PROPERTY(int writeModel READ writeModel WRITE setWriteModel
               NOTIFY writeModelChanged)
public:
    SAKBleCentralTool(QObject *parent = nullptr);
    ~SAKBleCentralTool();

public:
    Q_INVOKABLE QString serviceName(QVariant service);
    Q_INVOKABLE QVariantList characteristics(QVariant service);
    Q_INVOKABLE QString characteristicName(QVariant characteristic);
    Q_INVOKABLE void readCharacteristic();
    Q_INVOKABLE void changeNotify();
    Q_INVOKABLE bool hasFlag(QVariant characteristic, int flag);

signals:
    void descriptorWritten(const QLowEnergyDescriptor &descriptor,
                           const QByteArray &newValue);
    void serviceDiscoveryStarted();
    void serviceDiscoveryFinished();

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void readBytes() final;
    virtual void uninitialize() final;

private:
    QLowEnergyController *mBleCentral{nullptr};

private:
    void onServiceDiscovered(const QBluetoothUuid &newService);
    void onServiceDiscoveryFinished();
    void onBleCentralErrorOccuured(QLowEnergyController::Error err);
    void onBleCentralConnected();
    void onBleCentralDisconnected();
    void onServiceObjectStateChanged(QLowEnergyService *service,
                                     QLowEnergyService::ServiceState newState);

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//Properties
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
    QBluetoothDeviceInfo mBluetoothDeviceInfo;
    QVector<QLowEnergyService*> mServices;
    int mServiceIndex{-1};
    int mCharacteristicIndex{-1};
    int mWriteModel;
};

#endif // EDBLECENTRAL_HPP
