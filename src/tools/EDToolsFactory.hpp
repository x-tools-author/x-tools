/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDTOOLSFACTORY_HPP
#define EDTOOLSFACTORY_HPP

#include <QObject>
#include "EDBaseTool.hpp"

class EDToolsFactory : public QObject
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
    explicit EDToolsFactory(QObject *parent = Q_NULLPTR);

public:
    static EDToolsFactory *instance();
    EDBaseTool *createTool(int type);

};

#endif // EDTOOLSFACTORY_HPP
