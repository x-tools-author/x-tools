/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "iouifactory.h"

#include "common/xtools.h"

#ifdef X_TOOLS_ENABLE_BLUETOOTH
#include "device/blecentralui.h"
#endif
#include "device/communicationui.h"
#ifdef X_TOOLS_ENABLE_SERIAL_PORT
#include "device/serialportui.h"
#endif
#include "device/tcpclientui.h"
#include "device/tcpserverui.h"
#include "device/udpclientui.h"
#include "device/udpserverui.h"
#ifdef X_TOOLS_ENABLE_WEB_SOCKET
#include "device/websocketclientui.h"
#include "device/websocketserverui.h"
#endif
#include "device/chartstestui.h"

IOUiFactory::IOUiFactory() {}

IOUiFactory &IOUiFactory::singleton()
{
    static IOUiFactory instance;
    return instance;
}

CommunicationUi *IOUiFactory::createDeviceUi(int type)
{
    switch (type) {
#ifdef X_TOOLS_ENABLE_SERIAL_PORT
    case static_cast<int>(CommunicationType::SerialPort):
        return new SerialPortUi();
#endif
#ifdef X_TOOLS_ENABLE_BLUETOOTH
    case static_cast<int>(CommunicationType::BleCentral):
        return new BleCentralUi();
#endif
    case static_cast<int>(CommunicationType::UdpClient):
        return new UdpClientUi();
    case static_cast<int>(CommunicationType::UdpServer):
        return new UdpServerUi();
    case static_cast<int>(CommunicationType::TcpClient):
        return new TcpClientUi();
    case static_cast<int>(CommunicationType::TcpServer):
        return new TcpServerUi();
#ifdef X_TOOLS_ENABLE_WEB_SOCKET
    case static_cast<int>(CommunicationType::WebSocketClient):
        return new WebSocketClientUi();
    case static_cast<int>(CommunicationType::WebSocketServer):
        return new WebSocketServerUi();
    case static_cast<int>(CommunicationType::ChartsTest):
        return new ChartsTestUi();
#endif
    default:
        return nullptr;
    }
}
