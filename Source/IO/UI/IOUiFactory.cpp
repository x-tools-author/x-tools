/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "IOUiFactory.h"

#include "../xIO.h"

#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
#include "Communication/BleCentralUi.h"
#endif
#include "Communication/CommunicationUi.h"
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
#include "Communication/SerialPortUi.h"
#endif
#include "Communication/TcpClientUi.h"
#include "Communication/TcpServerUi.h"
#include "Communication/UdpClientUi.h"
#include "Communication/UdpServerUi.h"
#ifdef X_TOOLS_ENABLE_MODULE_WEB_SOCKET
#include "Communication/WebSocketClientUi.h"
#include "Communication/WebSocketServerUi.h"
#endif
#include "Communication/ChartsTestUi.h"

namespace xTools {

IOUiFactory::IOUiFactory() {}

IOUiFactory &IOUiFactory::singleton()
{
    static IOUiFactory instance;
    return instance;
}

CommunicationUi *IOUiFactory::createDeviceUi(int type)
{
    switch (type) {
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    case static_cast<int>(xIO::CommunicationType::SerialPort):
        return new SerialPortUi();
#endif
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
    case static_cast<int>(xIO::CommunicationType::BleCentral):
        return new BleCentralUi();
#endif
    case static_cast<int>(xIO::CommunicationType::UdpClient):
        return new UdpClientUi();
    case static_cast<int>(xIO::CommunicationType::UdpServer):
        return new UdpServerUi();
    case static_cast<int>(xIO::CommunicationType::TcpClient):
        return new TcpClientUi();
    case static_cast<int>(xIO::CommunicationType::TcpServer):
        return new TcpServerUi();
#ifdef X_TOOLS_ENABLE_MODULE_WEB_SOCKET
    case static_cast<int>(xIO::CommunicationType::WebSocketClient):
        return new WebSocketClientUi();
    case static_cast<int>(xIO::CommunicationType::WebSocketServer):
        return new WebSocketServerUi();
    case static_cast<int>(xIO::CommunicationType::ChartsTest):
        return new ChartsTestUi();
#endif
    default:
        return nullptr;
    }
}

} // namespace xTools
