/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>

#include "xToolsBaseTool.h"

class xToolsToolFactory : public QObject
{
    Q_OBJECT
public:
    enum ToolsType {
        AnalyzerTool,
        SerialPortTool,
        EmitterTool,
        MaskerTool,
        ResponserTool,
        DataPersistenceTool,
        DataListTool,
        SpeedometerTool,
        UdpClientTool,
        UdpServerTool,
        TcpClientTool,
        TcpServerTool,
        WebSocketClientTool,
        WebSocketServerTool,
        BleCentralTool,
        BlePeripheralTool,
        CounterTool,
        SerialPortTransmitterTool,
        UdpTransmitterTool,
        TcpTransmitterTool,
        WebSocketTransmitterTool,
        CrcCalculatorTool,
        HidTool,
        UnknownTool = -1
    };
    Q_ENUM(ToolsType)

private:
    explicit xToolsToolFactory(QObject *parent = Q_NULLPTR);

public:
    static xToolsToolFactory *instance();
    Q_INVOKABLE xToolsBaseTool *createTool(int type);
    QString toolName(int type);
    QString rawToolName(int type);
};
