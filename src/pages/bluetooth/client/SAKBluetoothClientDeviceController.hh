/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKBLUETOOTHCLIENTDEVICECONTROLLER_HH
#define SAKBLUETOOTHCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKBluetoothClientDeviceController;
}

class SAKDebugPage;
class SAKBluetoothClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct BluetoothClientParameters {
        QBluetoothDeviceInfo deviceInfo;
    };

    SAKBluetoothClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKBluetoothClientDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;
private:
    QMutex mParametersMutex;
    BluetoothClientParameters mParameters;
    QBluetoothDeviceDiscoveryAgent *mBluetoothDeviceDiscoveryAgent;
private:
    Ui::SAKBluetoothClientDeviceController *mUi;
    QComboBox *mRemoteDeviceComboBox;
};
Q_DECLARE_METATYPE(SAKBluetoothClientDeviceController::BluetoothClientParameters);
#endif
