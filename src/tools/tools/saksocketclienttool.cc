/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saksocketclienttool.h"

SAKSocketClientTool::SAKSocketClientTool(QObject *parent)
    : SAKCommunicationTool{parent}
{}

QString SAKSocketClientTool::clientIp()
{
    return m_clientIp;
}

void SAKSocketClientTool::setClientIp(const QString &ip)
{
    m_clientIp = ip;
    emit clientIpChanged();
}

int SAKSocketClientTool::clientPort()
{
    return m_clientPort;
}

void SAKSocketClientTool::setClientPort(int port)
{
    m_clientPort = port;
    emit clientPortChanged();
}

bool SAKSocketClientTool::specifyClientIpPort()
{
    return m_specifyClientIpPort;
}

void SAKSocketClientTool::setSpecifyClientIpPort(bool specified)
{
    m_specifyClientIpPort = specified;
}

QString SAKSocketClientTool::serverIp()
{
    return m_serverIp;
}

void SAKSocketClientTool::setServerIp(const QString &ip)
{
    m_serverIp = ip;
    emit serverIpChanged();
}

int SAKSocketClientTool::serverPort()
{
    return m_serverPort;
}

void SAKSocketClientTool::setServerPort(int port)
{
    m_serverPort = port;
    emit serverPortChanged();
}

QString SAKSocketClientTool::bindingIpPort()
{
    return m_bindingIpPort;
}

int SAKSocketClientTool::messageType()
{
    return m_messageType;
}

void SAKSocketClientTool::setMessageType(int type)
{
    m_messageType = type;
    emit messageTypeChanged();
}
