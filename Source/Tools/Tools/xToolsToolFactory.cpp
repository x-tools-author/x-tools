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
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
#include "xToolsSerialPortTool.h"
#include "xToolsSerialPortTransmitterTool.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_HID
#include "xToolsHidTool.h"
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
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
        tool = new xToolsSerialPortTool();
#endif
    } else if (EmitterTool == type) {
        tool = new xToolsEmitterTool();
    } else if (MaskerTool == type) {
        tool = new xToolsMaskerTool();
    } else if (ResponserTool == type) {
        tool = new xToolsResponserTool();
    } else if (DataPersistenceTool == type) {
        tool = new xToolsStorerTool();
    } else if (DataListTool == type) {
        tool = new xToolsPrestorerTool();
    } else if (SpeedometerTool == type) {
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
    } else if (BleCentralTool == type) {
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
        tool = new xToolsBleCentralTool();
#endif
    } else if (CounterTool == type) {
        tool = new xToolsStatisticianTool();
    } else if (SerialPortTransmitterTool == type) {
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
        tool = new xToolsSerialPortTransmitterTool();
#endif
    } else if (UdpTransmitterTool == type) {
        tool = new xToolsUdpTransmitterTool();
    } else if (TcpTransmitterTool == type) {
        tool = new xToolsTcpTransmitterTool();
    } else if (WebSocketTransmitterTool == type) {
        tool = new xToolsWebSocketTransmitterTool();
    } else if (CrcCalculatorTool == type) {
        tool = new xToolsCrcCalculatorTool();
    } else if (HidTool == type) {
#ifdef X_TOOLS_ENABLE_MODULE_HID
        tool = new xToolsCrcCalculatorTool();
#endif
    } else {
        qWarning() << "Unknown tool type: " << type;
    }

    if (tool) {
        QMetaEnum metaEnum = QMetaEnum::fromType<xToolsToolFactory::ToolsType>();
        tool->setProperty("toolTypeName", metaEnum.valueToKey(type));
    } else {
        QString msg = QString("Unknow tool type: %1.").arg(type);
        qWarning() << qPrintable(msg);
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
        map.insert(SerialPortTool, tr("Serial Port"));
        map.insert(EmitterTool, tr("Emitter"));
        map.insert(MaskerTool, tr("Masker"));
        map.insert(ResponserTool, tr("Responser"));
        map.insert(DataPersistenceTool, tr("Storer"));
        map.insert(DataListTool, tr("Prestore"));
        map.insert(SpeedometerTool, tr("Velometer"));
        map.insert(UdpClientTool, tr("UDP Client"));
        map.insert(UdpServerTool, tr("UDP Server"));
        map.insert(TcpClientTool, tr("TCP Client"));
        map.insert(TcpServerTool, tr("TCP Server"));
        map.insert(WebSocketClientTool, tr("Web Socket Client"));
        map.insert(WebSocketServerTool, tr("Web Socket Server"));
        map.insert(BleCentralTool, tr("BLE Central"));
        map.insert(BlePeripheralTool, tr("BLE Periheral"));
        map.insert(CounterTool, tr("Statistitician"));
        map.insert(SerialPortTransmitterTool, tr("Serial Port Transmitter"));
        map.insert(UdpTransmitterTool, tr("UDP Transmitter"));
        map.insert(TcpTransmitterTool, tr("TCP Transmitter"));
        map.insert(WebSocketTransmitterTool, tr("Web Socket Transmitter"));
        map.insert(CrcCalculatorTool, tr("CRC Calculator"));
    }

    return map.value(type, QString(""));
}

QString xToolsToolFactory::rawToolName(int type)
{
    auto metaEnum = QMetaEnum::fromType<xToolsToolFactory::ToolsType>();
    auto rawName = QString(metaEnum.valueToKey(type));
    return rawName.isEmpty() ? QString::number(type) : rawName;
}
