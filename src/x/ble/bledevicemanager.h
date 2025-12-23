/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <simpleble/Adapter.h>

#include <QThread>

#include "common/xdq.h"

#define xBleDevMgr BleDeviceManager::singleton()

namespace xBle {

class BleDeviceManagerPrivate;
class BleDeviceManager : public QObject
{
    Q_OBJECT
    X_DECLARE_PRIVATE(BleDeviceManager)

private:
    BleDeviceManager(QObject* parent = nullptr);
    BleDeviceManager(const BleDeviceManager&) = delete;
    BleDeviceManager& operator=(const BleDeviceManager&) = delete;

public:
    ~BleDeviceManager();
    static BleDeviceManager& singleton();

    void startScan();
    void stopScan();

signals:
    void scanStarted();
    void scanStopped();
    void errorOccurred(const QString& errorString);
    void deviceFound(const SimpleBLE::Peripheral& peripheral);
    void deviceUpdated(const SimpleBLE::Peripheral& peripheral);
};

} // namespace xBle