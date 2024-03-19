/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolBoxUiFactory.h"

#include "saktoolfactory.h"
#if 0
#include "SAKBleCentralToolBoxUi.h"
#include "SAKBlePeripheralToolBoxUi.h"
#include "SAKSerialPortToolBoxUi.h"
#include "SAKTcpClientToolBoxUi.h"
#include "SAKTcpServerToolBoxUi.h"
#include "SAKUdpClientToolBoxUi.h"
#include "SAKUdpServerToolBoxUi.h"
#include "SAKWebSocketClientToolBoxUi.h"
#include "SAKWebSocketServerToolBoxUi.h"
#endif

xToolsToolBoxUiFactory::xToolsToolBoxUiFactory(QObject* parent)
    : QObject{parent}
{}

xToolsToolBoxUiFactory* xToolsToolBoxUiFactory::instance()
{
    static xToolsToolBoxUiFactory factory;
    return &factory;
}

QList<int> xToolsToolBoxUiFactory::supportedTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool << SAKToolFactory::UdpClientTool
         << SAKToolFactory::UdpServerTool << SAKToolFactory::TcpClientTool
         << SAKToolFactory::TcpServerTool << SAKToolFactory::WebSocketClientTool
         << SAKToolFactory::WebSocketServerTool << SAKToolFactory::BleCentralTool
         << SAKToolFactory::BlePeriheral;
    return list;
}

xToolsToolBoxUi* xToolsToolBoxUiFactory::createToolBoxUi(int type)
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
