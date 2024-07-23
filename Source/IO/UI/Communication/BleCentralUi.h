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
#include <QLowEnergyService>

#include "CommunicationUi.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BleCentralUi;
}
QT_END_NAMESPACE

class BleScanner;
class BleCentralUi : public CommunicationUi
{
    Q_OBJECT
public:
    explicit BleCentralUi(xIO::CommunicationType type, QWidget *parent = nullptr);
    ~BleCentralUi() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupDevice(Communication *device) override;
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

    QByteArray notifyValue() const;
    QByteArray disnotifyValue() const;
};
