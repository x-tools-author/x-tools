/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "DeviceUiFactory.h"

#include "../xIO.h"

#include "Communication/BleCentralUi.h"
#include "Communication/CommunicationUi.h"
#include "Communication/SerialPortUi.h"
#include "Communication/TcpClientUi.h"
#include "Communication/TcpServerUi.h"
#include "Communication/UdpClientUi.h"
#include "Communication/UdpServerUi.h"
#include "Communication/WebSocketClientUi.h"
#include "Communication/WebSocketServerUi.h"

DeviceUiFactory::DeviceUiFactory() {}

DeviceUiFactory &DeviceUiFactory::singleton()
{
    static DeviceUiFactory instance;
    return instance;
}

CommunicationUi *DeviceUiFactory::createDeviceUi(xIO::DeviceType type)
{
    switch (type) {
    case xIO::DeviceType::SerialPort:
        return new SerialPortUi(type);
    case xIO::DeviceType::BleCentral:
        return new BleCentralUi(type);
    case xIO::DeviceType::UdpClient:
        return new UdpClientUi(type);
    case xIO::DeviceType::UdpServer:
        return new UdpServerUi(type);
    case xIO::DeviceType::TcpClient:
        return new TcpClientUi(type);
    case xIO::DeviceType::TcpServer:
        return new TcpServerUi(type);
    case xIO::DeviceType::WebSocketClient:
        return new WebSocketClientUi(type);
    case xIO::DeviceType::WebSocketServer:
        return new WebSocketServerUi(type);
    default:
        return nullptr;
    }
}
