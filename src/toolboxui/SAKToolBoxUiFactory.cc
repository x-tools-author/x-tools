/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolFactory.hh"
#include "SAKToolBoxUiFactory.hh"
#if 0
#include "SAKTcpClientToolBoxUi.hh"
#include "SAKTcpServerToolBoxUi.hh"
#include "SAKUdpClientToolBoxUi.hh"
#include "SAKUdpServerToolBoxUi.hh"
#include "SAKSerialPortToolBoxUi.hh"
#include "SAKBleCentralToolBoxUi.hh"
#include "SAKBlePeripheralToolBoxUi.hh"
#include "SAKWebSocketClientToolBoxUi.hh"
#include "SAKWebSocketServerToolBoxUi.hh"
#endif

SAKToolBoxUiFactory::SAKToolBoxUiFactory(QObject *parent)
    : QObject{parent}
{

}

SAKToolBoxUiFactory *SAKToolBoxUiFactory::instance()
{
    static SAKToolBoxUiFactory factory;
    return &factory;
}

QList<int> SAKToolBoxUiFactory::supportedTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool
         << SAKToolFactory::UdpClientTool
         << SAKToolFactory::UdpServerTool
         << SAKToolFactory::TcpClientTool
         << SAKToolFactory::TcpServerTool
         << SAKToolFactory::WebSocketClientTool
         << SAKToolFactory::WebSocketServerTool
         << SAKToolFactory::BleCentralTool
         << SAKToolFactory::BlePeriheral;
    return list;
}

SAKToolBoxUi *SAKToolBoxUiFactory::createToolBoxUi(int type)
{
#if 0
    if (type == SAKToolFactory::SerialportTool) {
        return new SAKSerialPortToolBoxUi();
    } else if (type == SAKToolFactory::BleCentral) {
        return new SAKBleCentralToolBoxUi();
    } else if (type == SAKToolFactory::BlePeriheral) {
        return new SAKBlePeripheralToolBoxUi();
    } else if (type == SAKToolFactory::TcpClientTool) {
        return new SAKTcpClientToolBoxUi();
    } else if (type == SAKToolFactory::TcpServerTool) {
        return new SAKTcpServerToolBoxUi();
    } else if (type == SAKToolFactory::UdpClientTool) {
        return new SAKUdpClientToolBoxUi();
    } else if (type == SAKToolFactory::UdpServerTool) {
        return new SAKUdpServerToolBoxUi();
    } else if (type == SAKToolFactory::WebSocketClientTool) {
        return new SAKWebSocketClientToolBoxUi();
    } else if (type == SAKToolFactory::WebSocketServerTool) {
        return new SAKWebSocketServerToolBoxUi();
    }
#endif

    return Q_NULLPTR;
}
