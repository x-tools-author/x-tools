/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKCommonDataStructure.hh"
#include "SAKBluetoothLowEnergyDevice.hh"
#include "SAKBluetoothLowEnergyDebugPage.hh"
#include "SAKBluetoothLowEnergyDeviceController.hh"

SAKBluetoothLowEnergyDebugPage::SAKBluetoothLowEnergyDebugPage(int type, QString name, QWidget *parent)
    :SAKDebugPage(type, name, parent)
{
    mDeviceController = new SAKBluetoothLowEnergyDeviceController(this);
    mDevice = new  SAKBluetoothLowEnergyDevice(this, this);
    initializePage();
}
