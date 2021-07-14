/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKBLUETOOTHLOWENERGYDEVICECONTROLLER_HH
#define SAKBLUETOOTHLOWENERGYDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKBluetoothLowEnergyDeviceController;
}

class SAKDebugPage;
class SAKBluetoothLowEnergyDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct BluetoothLowEnergyParameters {
        QBluetoothDeviceInfo deviceInfo;
    };

    SAKBluetoothLowEnergyDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKBluetoothLowEnergyDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;
private:
    QMutex mParametersMutex;
    BluetoothLowEnergyParameters mParameters;
    QBluetoothDeviceDiscoveryAgent *mBluetoothDeviceDiscoveryAgent;
private:
    Ui::SAKBluetoothLowEnergyDeviceController *mUi;
    QComboBox *mRemoteDeviceComboBox;
};
Q_DECLARE_METATYPE(SAKBluetoothLowEnergyDeviceController::BluetoothLowEnergyParameters);
#endif
