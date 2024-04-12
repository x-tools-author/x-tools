/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolFactory.h"

#include <QMetaEnum>

#include "xToolsAnalyzerTool.h"
#include "xToolsCrcCalculatorTool.h"
#include "xToolsEmitterTool.h"
#include "xToolsMaskerTool.h"
#include "xToolsPrestorerTool.h"
#include "xToolsResponserTool.h"
#include "xToolsSerialPortTool.h"
#include "xToolsSerialPortTransmitterTool.h"
#include "xToolsStatisticianTool.h"
#include "xToolsStorerTool.h"
#include "xToolsTcpClientTool.h"
#include "xToolsTcpServerTool.h"
#include "xToolsTcpTransmitterTool.h"
#include "xToolsUdpClientTool.h"
#include "xToolsUdpServerTool.h"
#include "xToolsUdpTransmitterTool.h"
#include "xToolsVelometerTool.h"
#include "xToolsWebSocketClientTool.h"
#include "xToolsWebSocketServerTool.h"
#include "xToolsWebSocketTransmitterTool.h"

#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
#include "xToolsBleCentralTool.h"
#endif

xToolsToolFactory::xToolsToolFactory(QObject *parent)
    : QObject{parent}
{}

xToolsToolFactory *xToolsToolFactory::instance()
{
    static xToolsToolFactory toolsFactory;
    return &toolsFactory;
}

xToolsBaseTool *xToolsToolFactory::createTool(int type)
{
    xToolsBaseTool *tool{nullptr};
    if (AnalyzerTool == type) {
        tool = new xToolsAnalyzerTool();
    } else if (SerialPortTool == type) {
        tool = new xToolsSerialPortTool();
    } else if (EmitterTool == type) {
        tool = new xToolsEmitterTool();
    } else if (MaskerTool == type) {
        tool = new xToolsMaskerTool();
    } else if (ResponserTool == type) {
        tool = new xToolsResponserTool();
    } else if (StorerTool == type) {
        tool = new xToolsStorerTool();
    } else if (PrestoreTool == type) {
        tool = new xToolsPrestorerTool();
    } else if (VelometerTool == type) {
        tool = new xToolsVelometerTool();
    } else if (UdpClientTool == type) {
        tool = new xToolsUdpClientTool();
    } else if (UdpServerTool == type) {
        tool = new xToolsUdpServerTool();
    } else if (TcpClientTool == type) {
        tool = new xToolsTcpClientTool();
    } else if (TcpServerTool == type) {
        tool = new xToolsTcpServerTool();
    } else if (WebSocketClientTool == type) {
        tool = new xToolsWebSocketClientTool();
    } else if (WebSocketServerTool == type) {
        tool = new xToolsWebSocketServerTool();
    }
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
    else if (BleCentralTool == type) {
        tool = new xToolsBleCentralTool();
    }
#endif
    else if (StatistiticianTool == type) {
        tool = new xToolsStatisticianTool();
    } else if (SerialPortTransmitterTool == type) {
        tool = new xToolsSerialPortTransmitterTool();
    } else if (UdpTransmitterTool == type) {
        tool = new xToolsUdpTransmitterTool();
    } else if (TcpTransmitterTool == type) {
        tool = new xToolsTcpTransmitterTool();
    } else if (WebSocketTransmitterTool == type) {
        tool = new xToolsWebSocketTransmitterTool();
    } else if (CrcCalculatorTool == type) {
        tool = new xToolsCrcCalculatorTool();
    } else {
        qWarning() << "Unknown tool type: " << type;
    }

    if (tool) {
        QMetaEnum metaEnum = QMetaEnum::fromType<xToolsToolFactory::ToolsType>();
        tool->setProperty("toolTypeName", metaEnum.valueToKey(type));
    } else {
        QString msg = QString("Unknow tool type: %1.").arg(type);
        Q_UNUSED(msg);
        Q_ASSERT_X(false, __FUNCTION__, msg.toLatin1().data());
    }

    if (tool) {
        tool->setToolType(type);
    }

    return tool;
}

QString xToolsToolFactory::toolName(int type)
{
    static QMap<int, QString> map;
    if (map.isEmpty()) {
        map.insert(AnalyzerTool, tr("Analyzer"));
        map.insert(SerialPortTool, tr("Serialport"));
        map.insert(EmitterTool, tr("Emitter"));
        map.insert(MaskerTool, tr("Masker"));
        map.insert(ResponserTool, tr("Responser"));
        map.insert(StorerTool, tr("Storer"));
        map.insert(PrestoreTool, tr("Prestore"));
        map.insert(VelometerTool, tr("Velometer"));
        map.insert(UdpClientTool, tr("UdpClient"));
        map.insert(UdpServerTool, tr("UdpServer"));
        map.insert(TcpClientTool, tr("TcpClient"));
        map.insert(TcpServerTool, tr("TcpServer"));
        map.insert(WebSocketClientTool, tr("WebSocketClient"));
        map.insert(WebSocketServerTool, tr("WebSocketServer"));
        map.insert(BleCentralTool, tr("BleCentral"));
        map.insert(BlePeriheral, tr("BlePeriheral"));
        map.insert(StatistiticianTool, tr("Statistitician"));
        map.insert(SerialPortTransmitterTool, tr("SerialPortTransmitter"));
        map.insert(UdpTransmitterTool, tr("UdpTransmitter"));
        map.insert(TcpTransmitterTool, tr("TcpTransmitter"));
        map.insert(WebSocketTransmitterTool, tr("WebSocketTransmitter"));
        map.insert(CrcCalculatorTool, tr("CrcCalculator"));
    }

    if (map.contains(type)) {
        return map.value(type);
    } else {
        return tr("Unknown");
    }
}
