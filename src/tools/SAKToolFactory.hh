/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLSFACTORY_HH
#define SAKTOOLSFACTORY_HH

#include <QObject>
#include "SAKBaseTool.hh"

class SAKToolsFactory : public QObject
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
    explicit SAKToolsFactory(QObject *parent = Q_NULLPTR);

public:
    static SAKToolsFactory *instance();
    SAKBaseTool *createTool(int type);

};

#endif // SAKTOOLSFACTORY_H
