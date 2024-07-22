/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "DeviceFactory.h"

#include <QCoreApplication>

#include "../xIO.h"

#include "Communication/BleCentral.h"
#include "Communication/SerialPort.h"
#include "Communication/TcpClient.h"
#include "Communication/TcpServer.h"
#include "Communication/UdpClient.h"
#include "Communication/UdpServer.h"
#include "Communication/WebSocketClient.h"
#include "Communication/WebSocketServer.h"

DeviceFactory::DeviceFactory() {}

DeviceFactory &DeviceFactory::singleton()
{
    static DeviceFactory instance;
    return instance;
}

Communication *DeviceFactory::createDevice(int type)
{
    switch (type) {
    case static_cast<int>(xIO::DeviceType::SerialPort):
        return new SerialPort(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::BleCentral):
        return new BleCentral(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::UdpClient):
        return new UdpClient(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::UdpServer):
        return new UdpServer(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::TcpClient):
        return new TcpClient(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::TcpServer):
        return new TcpServer(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::WebSocketClient):
        return new WebSocketClient(QCoreApplication::instance());
    case static_cast<int>(xIO::DeviceType::WebSocketServer):
        return new WebSocketServer(QCoreApplication::instance());
    default:
        qWarning("Unknown device type:%d", type);
        return nullptr;
    }
}
