/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMetaEnum>
#include "SAKMaskerTool.hh"
#include "SAKStorerTool.hh"
#include "SAKToolFactory.hh"
#include "SAKEmitterTool.hh"
#include "SAKAnalyzerTool.hh"
#include "SAKPrestorerTool.hh"
#include "SAKResponserTool.hh"
#include "SAKUdpClientTool.hh"
#include "SAKUdpServerTool.hh"
#include "SAKTcpClientTool.hh"
#include "SAKTcpServerTool.hh"
#include "SAKVelometerTool.hh"
#include "SAKBleCentralTool.hh"
#include "SAKSerialPortTool.hh"
#include "SAKWebSocketClientTool.hh"
#include "SAKWebSocketServerTool.hh"

SAKToolFactory::SAKToolFactory(QObject *parent)
    : QObject{parent}
{

}

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
        tool = new SAKVelometerTool;
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
    } else if (BleCentralTool == type) {
        tool = new SAKBleCentralTool();
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


    return tool;
}
