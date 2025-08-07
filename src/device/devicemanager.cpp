/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "devicemanager.h"

#include <QComboBox>

#include "localserverui.h"
#include "localsocketui.h"
#include "tcpclientui.h"
#include "tcpserverui.h"
#include "udpbroadcastui.h"
#include "udpclientui.h"
#include "udpmulticastui.h"
#include "udpserverui.h"

#ifdef X_ENABLE_SERIALPORT
#include "serialportui.h"
#endif
#ifdef X_ENABLE_WEBSOCKETS
#include "websocketclientui.h"
#include "websocketserverui.h"
#endif
#ifdef X_ENABLE_BLUETOOTH
#include "blecentralui.h"
#endif
#ifdef X_ENABLE_CHARTS
#include "chartstestui.h"
#endif

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
{}

DeviceManager::~DeviceManager() {}

DeviceManager &DeviceManager::singleton()
{
    static DeviceManager instance;
    return instance;
}

QList<int> DeviceManager::supportedDeviceTypes()
{
    static QList<int> deviceTypes;
    if (deviceTypes.isEmpty()) {
#ifdef X_ENABLE_SERIALPORT
        deviceTypes << static_cast<int>(DeviceType::SerialPort);
#endif
#ifdef X_ENABLE_BLUETOOTH
        deviceTypes << static_cast<int>(DeviceType::BleCentral);
#endif
#if 0
        deviceTypes << static_cast<int>(DeviceType::BlePeripheral);
#endif
        deviceTypes << static_cast<int>(DeviceType::UdpClient);
        deviceTypes << static_cast<int>(DeviceType::UdpServer);
        deviceTypes << static_cast<int>(DeviceType::UdpMulticast);
        deviceTypes << static_cast<int>(DeviceType::UdpBroadcast);
        deviceTypes << static_cast<int>(DeviceType::TcpClient);
        deviceTypes << static_cast<int>(DeviceType::TcpServer);
#ifdef X_ENABLE_WEBSOCKETS
        deviceTypes << static_cast<int>(DeviceType::WebSocketClient);
        deviceTypes << static_cast<int>(DeviceType::WebSocketServer);
#endif
        deviceTypes << static_cast<int>(DeviceType::LocalSocket);
        deviceTypes << static_cast<int>(DeviceType::LocalServer);
#ifdef X_ENABLE_CHARTS
        deviceTypes << static_cast<int>(DeviceType::ChartsTest);
#endif
    }

    return deviceTypes;
}

QString DeviceManager::deviceName(int type)
{
    switch (type) {
    case static_cast<int>(DeviceType::SerialPort):
        return QObject::tr("Serial Port");
    case static_cast<int>(DeviceType::BleCentral):
        return QObject::tr("BLE Central");
    case static_cast<int>(DeviceType::BlePeripheral):
        return QObject::tr("BLE Peripheral");
    case static_cast<int>(DeviceType::UdpClient):
        return QObject::tr("UDP Client");
    case static_cast<int>(DeviceType::UdpServer):
        return QObject::tr("UDP Server");
    case static_cast<int>(DeviceType::UdpMulticast):
        return QObject::tr("UDP Multicast");
    case static_cast<int>(DeviceType::UdpBroadcast):
        return QObject::tr("UDP Broadcast");
    case static_cast<int>(DeviceType::TcpClient):
        return QObject::tr("TCP Client");
    case static_cast<int>(DeviceType::TcpServer):
        return QObject::tr("TCP Server");
    case static_cast<int>(DeviceType::WebSocketClient):
        return QObject::tr("WebSocket Client");
    case static_cast<int>(DeviceType::WebSocketServer):
        return QObject::tr("WebSocket Server");
    case static_cast<int>(DeviceType::LocalSocket):
        return QObject::tr("Local Socket");
    case static_cast<int>(DeviceType::LocalServer):
        return QObject::tr("Local Server");
    case static_cast<int>(DeviceType::ChartsTest):
        return QObject::tr("Charts Test");
    default:
        return "Unknown";
    }
}

void DeviceManager::setupDeviceTypes(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> deviceTypes = supportedDeviceTypes();
    for (int &type : deviceTypes) {
        comboBox->addItem(deviceName(type), type);
    }

    comboBox->setCurrentIndex(comboBox->findData(static_cast<int>(DeviceType::SerialPort)));

    int udpClientIndex = comboBox->findData(static_cast<int>(DeviceType::UdpClient));
    if (udpClientIndex != -1) {
        comboBox->insertSeparator(udpClientIndex);
    }

    int testIndex = comboBox->findData(static_cast<int>(DeviceType::ChartsTest));
    if (testIndex != -1) {
        comboBox->insertSeparator(testIndex);
    }
}

DeviceUi *DeviceManager::newDeviceUi(int type)
{
    switch (type) {
#ifdef X_ENABLE_SERIALPORT
    case static_cast<int>(DeviceManager::SerialPort):
        return new SerialPortUi();
#endif
#ifdef X_ENABLE_BLUETOOTH
    case static_cast<int>(DeviceManager::BleCentral):
        return new BleCentralUi();
#endif
    case static_cast<int>(DeviceManager::UdpClient):
        return new UdpClientUi();
    case static_cast<int>(DeviceManager::UdpServer):
        return new UdpServerUi();
    case static_cast<int>(DeviceManager::UdpMulticast):
        return new UdpMulticastUi();
    case static_cast<int>(DeviceManager::UdpBroadcast):
        return new UdpBroadcastUi();
    case static_cast<int>(DeviceManager::TcpClient):
        return new TcpClientUi();
    case static_cast<int>(DeviceManager::TcpServer):
        return new TcpServerUi();
#ifdef X_ENABLE_WEBSOCKETS
    case static_cast<int>(DeviceManager::WebSocketClient):
        return new WebSocketClientUi();
    case static_cast<int>(DeviceManager::WebSocketServer):
        return new WebSocketServerUi();
#endif
    case static_cast<int>(DeviceManager::LocalSocket):
        return new LocalSocketUi();
    case static_cast<int>(DeviceManager::LocalServer):
        return new LocalServerUi();
#ifdef X_ENABLE_CHARTS
    case static_cast<int>(DeviceManager::ChartsTest):
        return new ChartsTestUi();
#endif
    default:
        return nullptr;
    }
}