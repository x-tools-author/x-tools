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
    return mServerIp;
}

void SAKSocketServerTool::setServerIp(const QString &ip)
{
    mServerIp = ip;
    emit serverIpChanged();
}

int SAKSocketServerTool::serverPort()
{
    return mServerPort;
}

void SAKSocketServerTool::setServerPort(int port)
{
    mServerPort = port;
    emit serverPortChanged();
}

bool SAKSocketServerTool::specifyIpAndPort()
{
    return mSpecifyIpAndPort;
}

void SAKSocketServerTool::setSpecifyIpAndPort(bool specified)
{
    mSpecifyIpAndPort = specified;
    emit specifyIpAndPortChanged();
}

QStringList SAKSocketServerTool::clients()
{
    return mClients;
}

int SAKSocketServerTool::clientIndex()
{
    return mClientIndex;
}

void SAKSocketServerTool::setClientIndex(int index)
{
    mClientIndex = index;
    emit clientIndexChanged();
}

QString SAKSocketServerTool::bindingIpPort()
{
    return mBindingIpPort;
}

int SAKSocketServerTool::messageType()
{
    return mMessageType;
}

void SAKSocketServerTool::setMessageType(int type)
{
    mMessageType = type;
    emit messageTypeChanged();
}
