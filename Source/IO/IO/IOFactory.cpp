/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "IOFactory.h"

#include <QCoreApplication>

#include "../xIO.h"
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
#include "Communication/BleCentral.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
#include "Communication/SerialPort.h"
#endif
#include "Communication/TcpClient.h"
#include "Communication/TcpServer.h"
#include "Communication/UdpClient.h"
#include "Communication/UdpServer.h"
#include "Communication/WebSocketClient.h"
#include "Communication/WebSocketServer.h"

namespace xTools {

IOFactory::IOFactory() {}

IOFactory &IOFactory::singleton()
{
    static IOFactory instance;
    return instance;
}

Communication *IOFactory::createDevice(int type)
{
    switch (type) {
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    case static_cast<int>(xIO::CommunicationType::SerialPort):
        return new SerialPort(QCoreApplication::instance());
#endif
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
    case static_cast<int>(xIO::CommunicationType::BleCentral):
        return new BleCentral(QCoreApplication::instance());
#endif
    case static_cast<int>(xIO::CommunicationType::UdpClient):
        return new UdpClient(QCoreApplication::instance());
    case static_cast<int>(xIO::CommunicationType::UdpServer):
        return new UdpServer(QCoreApplication::instance());
    case static_cast<int>(xIO::CommunicationType::TcpClient):
        return new TcpClient(QCoreApplication::instance());
    case static_cast<int>(xIO::CommunicationType::TcpServer):
        return new TcpServer(QCoreApplication::instance());
    case static_cast<int>(xIO::CommunicationType::WebSocketClient):
        return new WebSocketClient(QCoreApplication::instance());
    case static_cast<int>(xIO::CommunicationType::WebSocketServer):
        return new WebSocketServer(QCoreApplication::instance());
    default:
        qWarning("Unknown device type:%d", type);
        return nullptr;
    }
}

} // namespace xTools
