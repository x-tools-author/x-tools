/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "iofactory.h"

#include <QCoreApplication>

#include "devicepage/common/xio.h"
#ifdef X_TOOLS_ENABLE_BLUETOOTH
#include "device/blecentral.h"
#endif
#ifdef X_TOOLS_ENABLE_SERIAL_PORT
#include "device/serialport.h"
#endif
#include "device/tcpclient.h"
#include "device/tcpserver.h"
#include "device/udpclient.h"
#include "device/udpserver.h"
#ifdef X_TOOLS_ENABLE_WEB_SOCKET
#include "device/websocketclient.h"
#include "device/websocketserver.h"
#endif
#include "device/chartstest.h"

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
#ifdef X_TOOLS_ENABLE_SERIAL_PORT
    case static_cast<int>(xIO::CommunicationType::SerialPort):
        return new SerialPort(QCoreApplication::instance());
#endif
#ifdef X_TOOLS_ENABLE_BLUETOOTH
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
#ifdef X_TOOLS_ENABLE_WEB_SOCKET
    case static_cast<int>(xIO::CommunicationType::WebSocketClient):
        return new WebSocketClient(QCoreApplication::instance());
    case static_cast<int>(xIO::CommunicationType::WebSocketServer):
        return new WebSocketServer(QCoreApplication::instance());
#endif
    case static_cast<int>(xIO::CommunicationType::ChartsTest):
        return new ChartsTest(QCoreApplication::instance());
    default:
        qWarning("Unknown device type:%d", type);
        return nullptr;
    }
}

} // namespace xTools
