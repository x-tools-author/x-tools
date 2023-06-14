/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
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
    : SAKSocketClientTransmitterTool{"SAK.UdpTransmitterTool", parent}
{

}

SAKSocketClientTool *SAKUdpTransmitterTool::createTool()
{
    SAKUdpClientTool *tool = new SAKUdpClientTool();
    return tool;
}
