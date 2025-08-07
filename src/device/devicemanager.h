/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>

#define xDevMgr DeviceManager::singleton()

class DeviceUi;
class QComboBox;
class DeviceManager : public QObject
{
    Q_OBJECT
private:
    DeviceManager(QObject *parent = nullptr);
    DeviceManager(const DeviceManager &) = delete;
    DeviceManager &operator=(const DeviceManager &) = delete;

public:
    ~DeviceManager();
    static DeviceManager &singleton();

public:
    enum DeviceType {
        SerialPort,
        BleCentral,
        BlePeripheral,
        UdpClient,
        UdpServer,
        TcpClient,
        TcpServer,
        WebSocketClient,
        WebSocketServer,
        LocalSocket,
        LocalServer,
        UdpMulticast,
        UdpBroadcast,
        //----------------------------------------------------------------------------------------------
        Hid = 0x00200000,
        SctpClient,
        SctpServer,
        //----------------------------------------------------------------------------------------------
        ChartsTest = 0x00300000
    };
    QList<int> supportedDeviceTypes();
    QString deviceName(int type);
    void setupDeviceTypes(QComboBox *comboBox);
    DeviceUi *newDeviceUi(int type);
};