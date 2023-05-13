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
#include "SAKMaskerTool.hh"
#include "SAKStorerTool.hh"
#include "SAKEmitterTool.hh"
#include "SAKAnalyzerTool.hh"
#include "SAKPrestorerTool.hh"
#include "SAKResponserTool.hh"
#include "SAKUdpClientTool.hh"
#include "SAKUdpServerTool.hh"
#include "SAKTcpClientTool.hh"
#include "SAKTcpServerTool.hh"
#include "SAKBleCentralTool.hh"
#include "SAKSerialPortTool.hh"
#include "SAKWebSocketClientTool.hh"
#include "SAKWebSocketServerTool.hh"

class SAKToolFactory : public QObject
{
    Q_OBJECT
public:
    enum EDEnumToolsType {
        AnalyzerTool,
        SerialportTool,
        EmitterTool,
        MaskerTool,
        ResponserTool,
        StorerTool,
        PrestoreTool,
        UdpClientTool,
        UdpServerTool,
        TcpClientTool,
        TcpServerTool,
        WebSocketClientTool,
        WebSocketServerTool,
        BleCentral,
        BlePeriheral
    };
    Q_ENUM(EDEnumToolsType);

private:
    explicit SAKToolFactory(QObject *parent = Q_NULLPTR);

public:
    static SAKToolFactory *instance();
    SAKBaseTool *createTool(int type);

};

#endif // SAKTOOLFACTORY_H
