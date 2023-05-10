/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef EDBLECENTRAL_HPP
#define EDBLECENTRAL_HPP

#include <QVariantList>
#include <QBluetoothUuid>
#include <QLowEnergyService>
#include <QLowEnergyController>

#include "SAKCommunicationTool.hh"

class SAKBleCentralTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QVariant bleInfo READ bleInfo WRITE setBleInfo NOTIFY bleInfoChanged)
    Q_PROPERTY(QVariantList bleServiceObjects READ bleServiceObjects NOTIFY bleServiceObjectsChanged)
    Q_PROPERTY(int currentServiceIndex READ currentServiceIndex WRITE setCurrentServiceIndex NOTIFY currentServiceIndexChanged)
    Q_PROPERTY(int currentCharacteristicIndex READ currentCharacteristicIndex WRITE setCurrentCharacteristicIndex NOTIFY currentCharacteristicIndexChanged)
    Q_PROPERTY(int writeModel READ writeModel WRITE setWriteModel NOTIFY writeModelChanged)
    Q_PROPERTY(QStringList serviceNames READ serviceNames NOTIFY serviceNamesChanged)
    Q_PROPERTY(QStringList characteristicNames READ characteristicNames NOTIFY characteristicNamesChanged)
public:
    SAKBleCentralTool(QObject *parent = nullptr);
    ~SAKBleCentralTool();

    Q_INVOKABLE QVariantList serviceCharacteristics(QVariant service);
    Q_INVOKABLE QString characteristicName(QVariant characteristic);
    Q_INVOKABLE QString serviceName(QVariant service);
    Q_INVOKABLE void readCharacteristic();
    Q_INVOKABLE void changeNotify();

signals:
    void descriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);

protected:
    virtual bool initialize() final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void readBytes() final;
    virtual void uninitialize() final;

private:
    QVariantList mServices;
    QLowEnergyController *mBleCentral{nullptr};

private:
    void onServiceDiscovered(const QBluetoothUuid &newService);
    void onServiceDiscoveryFinished();
    void onBleCentralErrorOccuured(QLowEnergyController::Error err);
    void onServiceObjectStateChanged(QLowEnergyService *service, QLowEnergyService::ServiceState newState);

private:
    QVariant mBleInfo;
    QVariant bleInfo(){return mBleInfo;}
    void setBleInfo(QVariant info){mBleInfo = info; emit bleInfoChanged();}
    Q_SIGNAL void bleInfoChanged();

    QVariantList mBleServiceObjects;
    QVariantList bleServiceObjects(){return mBleServiceObjects;}
    Q_SIGNAL void bleServiceObjectsChanged();

    int mCurrentServiceIndex{-1};
    int currentServiceIndex(){return mCurrentServiceIndex;}
    void setCurrentServiceIndex(int index){mCurrentServiceIndex = index; emit currentServiceIndexChanged();}
    Q_SIGNAL void currentServiceIndexChanged();

    int mCurrentCharacteristicIndex{-1};
    int currentCharacteristicIndex(){return mCurrentCharacteristicIndex;}
    void setCurrentCharacteristicIndex(int index){mCurrentCharacteristicIndex = index; emit currentCharacteristicIndexChanged();}
    Q_SIGNAL void currentCharacteristicIndexChanged();

    int mWriteModel;
    int writeModel(){return mWriteModel;}
    void setWriteModel(int model){mWriteModel = model; emit writeModelChanged();}
    Q_SIGNAL void writeModelChanged();

    QStringList mServiceNames;
    QStringList serviceNames(){return mServiceNames;}
    Q_SIGNAL void serviceNamesChanged();

    QStringList mCharacteristicNames;
    QStringList characteristicNames(){return mCharacteristicNames;}
    Q_SIGNAL void characteristicNamesChanged();
};

#endif // EDBLECENTRAL_HPP
