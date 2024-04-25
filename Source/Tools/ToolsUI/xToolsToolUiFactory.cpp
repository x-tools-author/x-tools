/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolUiFactory.h"

#include <QCoreApplication>

#include "xToolsAnalyzerToolUi.h"
#include "xToolsCrcCalculatorToolUi.h"
#include "xToolsEmitterToolUi.h"
#include "xToolsMaskerToolUi.h"
#include "xToolsPrestorerToolUi.h"
#include "xToolsResponserToolUi.h"
#include "xToolsSocketClientToolUi.h"
#include "xToolsSocketServerToolUi.h"
#include "xToolsStatisticianToolUi.h"
#include "xToolsStorerToolUi.h"
#include "xToolsTcpTransmitterToolUi.h"
#include "xToolsToolFactory.h"
#include "xToolsUdpTransmitterToolUi.h"
#include "xToolsVelometerToolUi.h"
#include "xToolsWebSocketTransmitterToolUi.h"

#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
#include "xToolsBleCentralToolUi.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
#include "xToolsSerialPortToolUi.h"
#include "xToolsSerialPortTransmitterToolUi.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_HID
#include "xToolsHidToolUi.h"
#endif

xToolsToolUiFactory::xToolsToolUiFactory(QObject *parent)
    : QObject{parent}
{}

xToolsToolUiFactory *xToolsToolUiFactory::instance()
{
    static xToolsToolUiFactory *factory = nullptr;
    if (factory == nullptr) {
        factory = new xToolsToolUiFactory(qApp);
    }

    return factory;
}

xToolsBaseToolUi *xToolsToolUiFactory::createToolUi(int type)
{
    switch (type) {
    case xToolsToolFactory::AnalyzerTool:
        return new xToolsAnalyzerToolUi();
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    case xToolsToolFactory::SerialPortTool:
        return new xToolsSerialPortToolUi();
#endif
    case xToolsToolFactory::EmitterTool:
        return new xToolsEmitterToolUi();
    case xToolsToolFactory::MaskerTool:
        return new xToolsMaskerToolUi();
    case xToolsToolFactory::ResponserTool:
        return new xToolsResponserToolUi();
    case xToolsToolFactory::DataPersistenceTool:
        return new xToolsStorerToolUi();
    case xToolsToolFactory::DataListTool:
        return new xToolsPrestorerToolUi();
    case xToolsToolFactory::SpeedometerTool:
        return new xToolsVelometerToolUi();
    case xToolsToolFactory::UdpClientTool:
        return new xToolsSocketClientToolUi();
    case xToolsToolFactory::UdpServerTool:
        return new xToolsSocketServerToolUi();
    case xToolsToolFactory::TcpClientTool:
        return new xToolsSocketClientToolUi();
    case xToolsToolFactory::TcpServerTool:
        return new xToolsSocketServerToolUi();
    case xToolsToolFactory::WebSocketClientTool:
        return new xToolsSocketClientToolUi();
    case xToolsToolFactory::WebSocketServerTool:
        return new xToolsSocketServerToolUi();
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
    case xToolsToolFactory::BleCentralTool:
        return new xToolsBleCentralToolUi();
#endif
    case xToolsToolFactory::CounterTool:
        return new xToolsStatisticianToolUi();
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    case xToolsToolFactory::SerialPortTransmitterTool:
        return new xToolsSerialPortTransmitterToolUi();
#endif
    case xToolsToolFactory::UdpTransmitterTool:
        return new xToolsUdpTransmitterToolUi();
    case xToolsToolFactory::TcpTransmitterTool:
        return new xToolsTcpTransmitterToolUi();
    case xToolsToolFactory::WebSocketTransmitterTool:
        return new xToolsWebSocketTransmitterToolUi();
    case xToolsToolFactory::CrcCalculatorTool:
        return new xToolsCrcCalculatorToolUi();
#ifdef X_TOOLS_ENABLE_MODULE_HID
    case xToolsToolFactory::HidTool:
        return new xToolsHidToolUi();
#endif
    default:
        return nullptr;
    }
}
