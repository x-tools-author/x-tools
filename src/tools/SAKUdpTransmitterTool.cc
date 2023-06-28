/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QJsonObject>

#include "SAKUdpClientTool.hh"
#include "SAKUdpTransmitterTool.hh"

SAKUdpTransmitterTool::SAKUdpTransmitterTool(QObject *parent)
    : SAKSocketClientTransmitterTool{"sak.udptransmittertool", parent}
{

}

SAKCommunicationTool *SAKUdpTransmitterTool::createTool()
{
    SAKCommunicationTool *tool = new SAKUdpClientTool();
    return tool;
}
