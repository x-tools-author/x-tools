/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktooluifactory.h"

#include <QCoreApplication>

#include "xToolsAnalyzerToolUi.h"
#include "sakcrccalculatortoolui.h"
#include "sakemittertoolui.h"
#include "sakmaskertoolui.h"
#include "sakprestorertoolui.h"
#include "sakresponsertoolui.h"
#include "sakserialporttoolui.h"
#include "sakserialporttransmittertoolui.h"
#include "saksocketclienttoolui.h"
#include "saksocketclienttransmittertoolui.h"
#include "saksocketservertoolui.h"
#include "sakstatisticiantoolui.h"
#include "sakstorertoolui.h"
#include "saktcptransmittertoolti.h"
#include "xToolsToolFactory.h"
#include "sakudptransmittertoolui.h"
#include "xToolsVelometerToolUi.h"
#include "xToolsWebSocketTransmitterToolUi.h"

#ifdef SAK_IMPORT_MODULE_BLUETOOTH
#include "sakblecentraltoolui.h"
#endif

SAKToolUiFactory::SAKToolUiFactory(QObject *parent)
    : QObject{parent}
{}

SAKToolUiFactory *SAKToolUiFactory::instance()
{
    static SAKToolUiFactory *factory = nullptr;
    if (factory == nullptr) {
        factory = new SAKToolUiFactory(qApp);
    }

    return factory;
}

SAKBaseToolUi *SAKToolUiFactory::createToolUi(int type)
{
    switch (type) {
    case xToolsToolFactory::AnalyzerTool:
        return new xToolsAnalyzerToolUi();
    case xToolsToolFactory::SerialportTool:
        return new SAKSerialPortToolUi();
    case xToolsToolFactory::EmitterTool:
        return new SAKEmitterToolUi();
    case xToolsToolFactory::MaskerTool:
        return new SAKMaskerToolUi();
    case xToolsToolFactory::ResponserTool:
        return new SAKResponserToolUi();
    case xToolsToolFactory::StorerTool:
        return new SAKStorerToolUi();
    case xToolsToolFactory::PrestoreTool:
        return new SAKPrestorerToolUi();
    case xToolsToolFactory::VelometerTool:
        return new xToolsVelometerToolUi();
    case xToolsToolFactory::UdpClientTool:
        return new SAKSocketClientToolUi();
    case xToolsToolFactory::UdpServerTool:
        return new SAKSocketServerToolUi();
    case xToolsToolFactory::TcpClientTool:
        return new SAKSocketClientToolUi();
    case xToolsToolFactory::TcpServerTool:
        return new SAKSocketServerToolUi();
    case xToolsToolFactory::WebSocketClientTool:
        return new SAKSocketClientToolUi();
    case xToolsToolFactory::WebSocketServerTool:
        return new SAKSocketServerToolUi();
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
    case SAKToolFactory::BleCentralTool:
        return new SAKBleCentralToolUi();
#endif
    case xToolsToolFactory::StatistiticianTool:
        return new SAKStatisticianToolUi();
    case xToolsToolFactory::SerialPortTransmitterTool:
        return new SAKSerialPortTransmitterToolUi();
    case xToolsToolFactory::UdpTransmitterTool:
        return new SAKUdpTransmitterToolUi();
    case xToolsToolFactory::TcpTransmitterTool:
        return new SAKTcpTransmitterToolUi();
    case xToolsToolFactory::WebSocketTransmitterTool:
        return new xToolsWebSocketTransmitterToolUi();
    case xToolsToolFactory::CrcCalculatorTool:
        return new SAKCrcCalculatorToolUi();
    default:
        return nullptr;
    }
}