/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakwebsockettransmittertool.h"
#include "sakwebsocketclienttool.h"

SAKWebSocketTransmitterTool::SAKWebSocketTransmitterTool(QObject *parent)
    : SAKSocketClientTransmitterTool{parent}
{}

SAKCommunicationTool *SAKWebSocketTransmitterTool::createTool()
{
    SAKWebSocketClientTool *tool = new SAKWebSocketClientTool();
    return tool;
}
