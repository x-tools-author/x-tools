/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSocketClientTool.hh"

SAKSocketClientTool::SAKSocketClientTool(const char *lg, QObject *parent)
    : SAKCommunicationTool{lg, parent}
{

}

QString SAKSocketClientTool::clientIp()
{
    return mClientIp;
}

void SAKSocketClientTool::setClientIp(const QString &ip)
{
    mClientIp = ip; emit clientIpChanged();
}

int SAKSocketClientTool::clientPort()
{
    return mClientPort;
}

void SAKSocketClientTool::setClientPort(int port)
{
    mClientPort = port; emit clientPortChanged();
}

bool SAKSocketClientTool::specifyClientIpPort()
{
    return mSpecifyClientIpPort;
}

void SAKSocketClientTool::setSpecifyClientIpPort(bool specified)
{
    mSpecifyClientIpPort = specified;
}

QString SAKSocketClientTool::serverIp()
{
    return mServerIp;
}

void SAKSocketClientTool::setServerIp(const QString &ip)
{
    mServerIp = ip;
    emit serverIpChanged();
}

int SAKSocketClientTool::serverPort()
{
    return mServerPort;
}

void SAKSocketClientTool::setServerPort(int port)
{
    mServerPort = port; emit serverPortChanged();
}

QString SAKSocketClientTool::bindingIpPort()
{
    return mBindingIpPort;
}

int SAKSocketClientTool::messageType()
{
    return mMessageType;
}

void SAKSocketClientTool::setMessageType(int type)
{
    mMessageType = type; emit messageTypeChanged();
}
