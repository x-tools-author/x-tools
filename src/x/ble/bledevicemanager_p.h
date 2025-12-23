/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QThread>

#include <simpleble/Adapter.h>

#include "bledevicemanager.h"
#include "common/xdq.h"

namespace xBle {

class BleDeviceManagerPrivate : public QThread
{
    Q_OBJECT
    X_DECLARE_PUBLIC(BleDeviceManager)
public:
    explicit BleDeviceManagerPrivate(BleDeviceManager* q)
        : QThread(q)
        , x_q_ptr(q)
    {}
    ~BleDeviceManagerPrivate() {}

protected:
    void run() override
    {
        std::optional<SimpleBLE::Adapter> adapter = getAdapter();
        if (!adapter.has_value()) {
            return;
        }

        adapter->set_callback_on_scan_found([=](SimpleBLE::Peripheral peripheral) {
            qInfo("Found device: %s [%s] %d dBm",
                  peripheral.identifier().c_str(),
                  peripheral.address().c_str(),
                  peripheral.rssi());
            emit x_q_ptr->deviceFound(peripheral);
        });

        adapter->set_callback_on_scan_updated([=](SimpleBLE::Peripheral peripheral) {
            qInfo("Updated device: %s [%s] %d dBm",
                  peripheral.identifier().c_str(),
                  peripheral.address().c_str(),
                  peripheral.rssi());
            emit x_q_ptr->deviceUpdated(peripheral);
        });

        adapter->set_callback_on_scan_start([=]() {
            qInfo("BLE scan started.");
            emit x_q_ptr->scanStarted();
        });

        adapter->set_callback_on_scan_stop([=]() {
            qInfo("BLE scan stopped.");
            emit x_q_ptr->scanStopped();
        });

        // Scan indefinitely until stopped.
        adapter->scan_for(10000); // Scan for 10 seconds for demo purpose
    }

    std::optional<SimpleBLE::Adapter> getAdapter()
    {
        if (!SimpleBLE::Adapter::bluetooth_enabled()) {
            qWarning("Bluetooth is not enabled!");
            return {};
        }

        auto adapter_list = SimpleBLE::Adapter::get_adapters();
        if (adapter_list.empty()) {
            qWarning("No adapter was found.");
            return {};
        }

        // only one found, returning directly
        if (adapter_list.size() == 1) {
            auto adapter = adapter_list.at(0);
            qInfo("Using adapter: %s [%s]", adapter.identifier().c_str(), adapter.address().c_str());
            return adapter;
        }

        // multiple adapters found, using the first one for now
        qInfo("Available adapters:");
        int i = 0;
        for (auto& adapter : adapter_list) {
            qInfo("[%d] %s [%s]", i++, adapter.identifier().c_str(), adapter.address().c_str());
        }

        if (adapter_list.empty()) {
            qWarning("No adapter was found.");
            return {};
        } else {
            return std::optional<SimpleBLE::Adapter>(adapter_list.at(0));
        }
    }
};

} // namespace xBle