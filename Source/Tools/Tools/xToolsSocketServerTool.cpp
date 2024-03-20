/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketServerTool.h"

xToolsSocketServerTool::xToolsSocketServerTool(QObject *parent)
    : xToolsCommunicationTool{parent}
{}

QString xToolsSocketServerTool::serverIp()
{
    return m_serverIp;
}

void xToolsSocketServerTool::setServerIp(const QString &ip)
{
    m_serverIp = ip;
    emit serverIpChanged();
}

int xToolsSocketServerTool::serverPort()
{
    return m_serverPort;
}

void xToolsSocketServerTool::setServerPort(int port)
{
    m_serverPort = port;
    emit serverPortChanged();
}

bool xToolsSocketServerTool::specifyIpAndPort()
{
    return m_specifyIpAndPort;
}

void xToolsSocketServerTool::setSpecifyIpAndPort(bool specified)
{
    m_specifyIpAndPort = specified;
    emit specifyIpAndPortChanged();
}

QStringList xToolsSocketServerTool::clients()
{
    return m_clients;
}

int xToolsSocketServerTool::clientIndex()
{
    return m_clientIndex;
}

void xToolsSocketServerTool::setClientIndex(int index)
{
    m_clientIndex = index;
    emit clientIndexChanged();
}

QString xToolsSocketServerTool::bindingIpPort()
{
    return m_bindingIpPort;
}

int xToolsSocketServerTool::messageType()
{
    return m_messageType;
}

void xToolsSocketServerTool::setMessageType(int type)
{
    m_messageType = type;
    emit messageTypeChanged();
}
