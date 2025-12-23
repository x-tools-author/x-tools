/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "bledevicemanager.h"
#include "bledevicemanager_p.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <QMetaType>

namespace xBle {

BleDeviceManager::BleDeviceManager(QObject *parent)
    : QObject(parent)
{
    x_d_ptr = new BleDeviceManagerPrivate(this);

    qRegisterMetaType<SimpleBLE::Peripheral>("SimpleBLE::Peripheral");
}

BleDeviceManager::~BleDeviceManager() {}

BleDeviceManager &BleDeviceManager::singleton()
{
    static BleDeviceManager instance;
    return instance;
}

void BleDeviceManager::startScan()
{
    if (!x_d_ptr->isRunning()) {
        x_d_ptr->start();
    }
}

void BleDeviceManager::stopScan()
{
    if (x_d_ptr->isRunning()) {
        x_d_ptr->quit();
        x_d_ptr->wait();
    }
}

} // namespace xBle