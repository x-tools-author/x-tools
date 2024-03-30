/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketClientTool.h"

xToolsSocketClientTool::xToolsSocketClientTool(QObject *parent)
    : xToolsCommunicationTool{parent}
{}

QString xToolsSocketClientTool::clientIp()
{
    return m_clientIp;
}

void xToolsSocketClientTool::setClientIp(const QString &ip)
{
    m_clientIp = ip;
    emit clientIpChanged();
}

int xToolsSocketClientTool::clientPort()
{
    return m_clientPort;
}

void xToolsSocketClientTool::setClientPort(int port)
{
    m_clientPort = port;
    emit clientPortChanged();
}

bool xToolsSocketClientTool::specifyClientIpPort()
{
    return m_specifyClientIpPort;
}

void xToolsSocketClientTool::setSpecifyClientIpPort(bool specified)
{
    m_specifyClientIpPort = specified;
}

QString xToolsSocketClientTool::serverIp()
{
    return m_serverIp;
}

void xToolsSocketClientTool::setServerIp(const QString &ip)
{
    m_serverIp = ip;
    emit serverIpChanged();
}

int xToolsSocketClientTool::serverPort()
{
    return m_serverPort;
}

void xToolsSocketClientTool::setServerPort(int port)
{
    m_serverPort = port;
    emit serverPortChanged();
}

QString xToolsSocketClientTool::bindingIpPort()
{
    return m_bindingIpPort;
}

int xToolsSocketClientTool::messageType()
{
    return m_messageType;
}

void xToolsSocketClientTool::setMessageType(int type)
{
    m_messageType = type;
    emit messageTypeChanged();
}

bool xToolsSocketClientTool::authentication()
{
    return m_authentication;
}

void xToolsSocketClientTool::setAuthentication(bool authentication)
{
    m_authentication = authentication;
    emit authenticationChanged();
}

QString xToolsSocketClientTool::userName()
{
    return m_userName;
}

void xToolsSocketClientTool::setUserName(const QString &userName)
{
    m_userName = userName;
    emit userNameChanged();
}

QString xToolsSocketClientTool::password()
{
    return m_password;
}

void xToolsSocketClientTool::setPassword(const QString &password)
{
    m_password = password;
    emit passwordChanged();
}
