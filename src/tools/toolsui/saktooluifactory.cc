/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktooluifactory.h"

#include <QCoreApplication>

#include "sakanalyzertoolui.h"
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
#include "saktoolfactory.h"
#include "sakudptransmittertoolui.h"
#include "sakvelometertoolui.h"
#include "sakwebsockettransmittertoolui.h"

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
    case SAKToolFactory::AnalyzerTool:
        return new SAKAnalyzerToolUi();
    case SAKToolFactory::SerialportTool:
        return new SAKSerialPortToolUi();
    case SAKToolFactory::EmitterTool:
        return new SAKEmitterToolUi();
    case SAKToolFactory::MaskerTool:
        return new SAKMaskerToolUi();
    case SAKToolFactory::ResponserTool:
        return new SAKResponserToolUi();
    case SAKToolFactory::StorerTool:
        return new SAKStorerToolUi();
    case SAKToolFactory::PrestoreTool:
        return new SAKPrestorerToolUi();
    case SAKToolFactory::VelometerTool:
        return new SAKVelometerToolUi();
    case SAKToolFactory::UdpClientTool:
        return new SAKSocketClientToolUi();
    case SAKToolFactory::UdpServerTool:
        return new SAKSocketServerToolUi();
    case SAKToolFactory::TcpClientTool:
        return new SAKSocketClientToolUi();
    case SAKToolFactory::TcpServerTool:
        return new SAKSocketServerToolUi();
    case SAKToolFactory::WebSocketClientTool:
        return new SAKSocketClientToolUi();
    case SAKToolFactory::WebSocketServerTool:
        return new SAKSocketServerToolUi();
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
    case SAKToolFactory::BleCentralTool:
        return new SAKBleCentralToolUi();
#endif
    case SAKToolFactory::StatistiticianTool:
        return new SAKStatisticianToolUi();
    case SAKToolFactory::SerialPortTransmitterTool:
        return new SAKSerialPortTransmitterToolUi();
    case SAKToolFactory::UdpTransmitterTool:
        return new SAKUdpTransmitterToolUi();
    case SAKToolFactory::TcpTransmitterTool:
        return new SAKTcpTransmitterToolUi();
    case SAKToolFactory::WebSocketTransmitterTool:
        return new SAKWebSocketTransmitterToolUi();
    case SAKToolFactory::CrcCalculatorTool:
        return new SAKCrcCalculatorToolUi();
    default:
        return nullptr;
    }
}