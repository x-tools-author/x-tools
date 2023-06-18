/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKTcpClientTool.hh"
#include "SAKTcpTransmitterTool.hh"

SAKTcpTransmitterTool::SAKTcpTransmitterTool(QObject *parent)
    : SAKSocketClientTransmitterTool{"SAK.TcpTransmitterTool", parent}
{

}

SAKCommunicationTool *SAKTcpTransmitterTool::createTool()
{
    SAKCommunicationTool *tool = new SAKTcpClientTool();
    return tool;
}
