/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDMaskerTool.hpp"
#include "EDStorerTool.hpp"
#include "EDEmitterTool.hpp"
#include "EDAnalyzerTool.hpp"
#include "EDToolsFactory.hpp"
#include "EDPrestorerTool.hpp"
#include "EDResponserTool.hpp"
#include "EDUdpClientTool.hpp"
#include "EDUdpServerTool.hpp"
#include "EDTcpClientTool.hpp"
#include "EDTcpServerTool.hpp"
#include "EDBleCentralTool.hpp"
#include "EDSerialPortTool.hpp"
#include "EDWebSocketClientTool.hpp"
#include "EDWebSocketServerTool.hpp"

EDToolsFactory::EDToolsFactory(QObject *parent)
    : QObject{parent}
{

}

EDToolsFactory *EDToolsFactory::instance()
{
    static EDToolsFactory toolsFactory;
    return &toolsFactory;
}

EDBaseTool *EDToolsFactory::createTool(int type)
{
    if (AnalyzerTool == type) {
        return new EDAnalyzerTool();
    } else if (SerialportTool == type) {
        return new EDSerialPortTool();
    } else if (EmitterTool == type) {
        return new EDEmitterTool();
    } else if (MaskerTool == type) {
        return new EDMaskerTool();
    } else if (ResponserTool == type) {
        return new EDResponserTool();
    } else if (StorerTool == type) {
        return new EDStorerTool();
    } else if (PrestoreTool == type) {
        return new EDPrestorerTool();
    } else if (UdpClientTool == type) {
        return new EDUdpClientTool();
    } else if (UdpServerTool == type) {
        return new EDUdpServerTool();
    } else if (TcpClientTool == type) {
        return new EDTcpClientTool();
    } else if (TcpServerTool == type) {
        return new EDTcpServerTool();
    } else if (WebSocketClientTool == type) {
        return new EDWebSocketClientTool();
    } else if (WebSocketServerTool == type) {
        return new EDWebSocketServerTool();
    } else if (BleCentral == type) {
        return new EDBleCentralTool();
    }

    QString msg = QString("Unknow tool type: %1.").arg(type);
    Q_UNUSED(msg);
    Q_ASSERT_X(false, __FUNCTION__, msg.toLatin1().data());
    return Q_NULLPTR;
}
