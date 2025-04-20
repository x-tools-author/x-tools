/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QBluetoothDeviceInfo>
#include <QLowEnergyService>

#include "deviceui.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BleCentralUi;
}
QT_END_NAMESPACE

class BleScanner;
class BleCentralUi : public DeviceUi
{
    Q_OBJECT
public:
    explicit BleCentralUi(QWidget *parent = nullptr);
    ~BleCentralUi() override;

    Device *newDevice() override;
    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setUiEnabled(bool enabled) override;

private:
    Ui::BleCentralUi *ui;
    BleScanner *m_scanner{nullptr};

private:
    void updateNotifyText();
    void resetVisible();

    void setNotifyText(const QByteArray &value);
    void setupWriteMode(bool enableWithResponse, bool writeWithoutResponse);
    void setupLowEnergyService(QLowEnergyService *service);

    void onScanButtonClicked();
    void onServiceIndexChanged(int index);
    void onCharacteristicIndexChanged(int index);
    void onDescriptorWritten(const QLowEnergyDescriptor &info, const QByteArray &value);
    void onDescriptorRead(const QLowEnergyDescriptor &info, const QByteArray &value);

    void onNotifyButtonClicked();
    void onReadButtonClicked();

    void onScanningStarted();
    void onScanningFinished();
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);
    void onErrorOccurred(const QString &errStr);

    QByteArray notifyValue() const;
    QByteArray disnotifyValue() const;
};
