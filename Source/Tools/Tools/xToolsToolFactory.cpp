/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolfactory.h"

#include <QMetaEnum>

#include "sakanalyzertool.h"
#include "sakcrccalculatortool.h"
#include "sakemittertool.h"
#include "sakmaskertool.h"
#include "sakprestorertool.h"
#include "sakresponsertool.h"
#include "sakserialporttool.h"
#include "sakserialporttransmittertool.h"
#include "sakstatisticiantool.h"
#include "sakstorertool.h"
#include "saktcpclienttool.h"
#include "saktcpservertool.h"
#include "saktcptransmittertool.h"
#include "sakudpclienttool.h"
#include "sakudpservertool.h"
#include "sakudptransmittertool.h"
#include "sakvelometertool.h"
#include "sakwebsocketclienttool.h"
#include "sakwebsocketservertool.h"
#include "sakwebsockettransmittertool.h"

#ifdef SAK_IMPORT_MODULE_BLUETOOTH
#include "sakblecentraltool.h"
#endif

SAKToolFactory::SAKToolFactory(QObject *parent)
    : QObject{parent}
{}

SAKToolFactory *SAKToolFactory::instance()
{
    static SAKToolFactory toolsFactory;
    return &toolsFactory;
}

SAKBaseTool *SAKToolFactory::createTool(int type)
{
    SAKBaseTool *tool{nullptr};
    if (AnalyzerTool == type) {
        tool = new SAKAnalyzerTool();
    } else if (SerialportTool == type) {
        tool = new SAKSerialPortTool();
    } else if (EmitterTool == type) {
        tool = new SAKEmitterTool();
    } else if (MaskerTool == type) {
        tool = new SAKMaskerTool();
    } else if (ResponserTool == type) {
        tool = new SAKResponserTool();
    } else if (StorerTool == type) {
        tool = new SAKStorerTool();
    } else if (PrestoreTool == type) {
        tool = new SAKPrestorerTool();
    } else if (VelometerTool == type) {
        tool = new SAKVelometerTool();
    } else if (UdpClientTool == type) {
        tool = new SAKUdpClientTool();
    } else if (UdpServerTool == type) {
        tool = new SAKUdpServerTool();
    } else if (TcpClientTool == type) {
        tool = new SAKTcpClientTool();
    } else if (TcpServerTool == type) {
        tool = new SAKTcpServerTool();
    } else if (WebSocketClientTool == type) {
        tool = new SAKWebSocketClientTool();
    } else if (WebSocketServerTool == type) {
        tool = new SAKWebSocketServerTool();
    }
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
    else if (BleCentralTool == type) {
        tool = new SAKBleCentralTool();
    }
#endif
    else if (StatistiticianTool == type) {
        tool = new SAKStatisticianTool();
    } else if (SerialPortTransmitterTool == type) {
        tool = new SAKSerialPortTransmitterTool();
    } else if (UdpTransmitterTool == type) {
        tool = new SAKUdpTransmitterTool();
    } else if (TcpTransmitterTool == type) {
        tool = new SAKTcpTransmitterTool();
    } else if (WebSocketTransmitterTool == type) {
        tool = new SAKWebSocketTransmitterTool();
    } else if (CrcCalculatorTool == type) {
        tool = new SAKCrcCalculatorTool();
    } else {
        qWarning() << "Unknown tool type: " << type;
    }

    if (tool) {
        QMetaEnum metaEnum = QMetaEnum::fromType<SAKToolFactory::ToolsType>();
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

QString SAKToolFactory::toolName(int type)
{
    static QMap<int, QString> map;
    if (map.isEmpty()) {
        map.insert(AnalyzerTool, tr("Analyzer"));
        map.insert(SerialportTool, tr("Serialport"));
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
