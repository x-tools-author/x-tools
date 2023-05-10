/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKMaskerTool.hh"
#include "SAKStorerTool.hh"
#include "SAKEmitterTool.hh"
#include "SAKAnalyzerTool.hh"
#include "SAKToolsFactory.hh"
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

SAKToolsFactory::SAKToolsFactory(QObject *parent)
    : QObject{parent}
{

}

SAKToolsFactory *SAKToolsFactory::instance()
{
    static SAKToolsFactory toolsFactory;
    return &toolsFactory;
}

SAKBaseTool *SAKToolsFactory::createTool(int type)
{
    if (AnalyzerTool == type) {
        return new SAKAnalyzerTool();
    } else if (SerialportTool == type) {
        return new SAKSerialPortTool();
    } else if (EmitterTool == type) {
        return new SAKEmitterTool();
    } else if (MaskerTool == type) {
        return new SAKMaskerTool();
    } else if (ResponserTool == type) {
        return new SAKResponserTool();
    } else if (StorerTool == type) {
        return new SAKStorerTool();
    } else if (PrestoreTool == type) {
        return new SAKPrestorerTool();
    } else if (UdpClientTool == type) {
        return new SAKUdpClientTool();
    } else if (UdpServerTool == type) {
        return new SAKUdpServerTool();
    } else if (TcpClientTool == type) {
        return new SAKTcpClientTool();
    } else if (TcpServerTool == type) {
        return new SAKTcpServerTool();
    } else if (WebSocketClientTool == type) {
        return new SAKWebSocketClientTool();
    } else if (WebSocketServerTool == type) {
        return new SAKWebSocketServerTool();
    } else if (BleCentral == type) {
        return new SAKBleCentralTool();
    }

    QString msg = QString("Unknow tool type: %1.").arg(type);
    Q_UNUSED(msg);
    Q_ASSERT_X(false, __FUNCTION__, msg.toLatin1().data());
    return Q_NULLPTR;
}
