/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLFACTORY_HH
#define SAKTOOLFACTORY_HH

#include <QObject>

#include "SAKBaseTool.hh"

class SAKToolFactory : public QObject
{
    Q_OBJECT
public:
    enum ToolsType {
        AnalyzerTool,
        SerialportTool,
        EmitterTool,
        MaskerTool,
        ResponserTool,
        StorerTool,
        PrestoreTool,
        VelometerTool,
        UdpClientTool,
        UdpServerTool,
        TcpClientTool,
        TcpServerTool,
        WebSocketClientTool,
        WebSocketServerTool,
        BleCentralTool,
        BlePeriheral,
        StatistiticianTool
    };
    Q_ENUM(ToolsType)

private:
    explicit SAKToolFactory(QObject *parent = Q_NULLPTR);

public:
    static SAKToolFactory *instance();
    SAKBaseTool *createTool(int type);

};

#endif // SAKTOOLFACTORY_H
