/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolboxuifactory.h"

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

SAKToolBoxUiFactory::SAKToolBoxUiFactory(QObject* parent)
    : QObject{parent}
{}

SAKToolBoxUiFactory* SAKToolBoxUiFactory::instance()
{
    static SAKToolBoxUiFactory factory;
    return &factory;
}

QList<int> SAKToolBoxUiFactory::supportedTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool << SAKToolFactory::UdpClientTool
         << SAKToolFactory::UdpServerTool << SAKToolFactory::TcpClientTool
         << SAKToolFactory::TcpServerTool << SAKToolFactory::WebSocketClientTool
         << SAKToolFactory::WebSocketServerTool << SAKToolFactory::BleCentralTool
         << SAKToolFactory::BlePeriheral;
    return list;
}

SAKToolBoxUi* SAKToolBoxUiFactory::createToolBoxUi(int type)
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
