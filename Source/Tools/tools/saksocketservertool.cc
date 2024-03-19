/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saksocketservertool.h"

SAKSocketServerTool::SAKSocketServerTool(QObject *parent)
    : SAKCommunicationTool{parent}
{}

QString SAKSocketServerTool::serverIp()
{
    return m_serverIp;
}

void SAKSocketServerTool::setServerIp(const QString &ip)
{
    m_serverIp = ip;
    emit serverIpChanged();
}

int SAKSocketServerTool::serverPort()
{
    return m_serverPort;
}

void SAKSocketServerTool::setServerPort(int port)
{
    m_serverPort = port;
    emit serverPortChanged();
}

bool SAKSocketServerTool::specifyIpAndPort()
{
    return m_specifyIpAndPort;
}

void SAKSocketServerTool::setSpecifyIpAndPort(bool specified)
{
    m_specifyIpAndPort = specified;
    emit specifyIpAndPortChanged();
}

QStringList SAKSocketServerTool::clients()
{
    return m_clients;
}

int SAKSocketServerTool::clientIndex()
{
    return m_clientIndex;
}

void SAKSocketServerTool::setClientIndex(int index)
{
    m_clientIndex = index;
    emit clientIndexChanged();
}

QString SAKSocketServerTool::bindingIpPort()
{
    return m_bindingIpPort;
}

int SAKSocketServerTool::messageType()
{
    return m_messageType;
}

void SAKSocketServerTool::setMessageType(int type)
{
    m_messageType = type;
    emit messageTypeChanged();
}
