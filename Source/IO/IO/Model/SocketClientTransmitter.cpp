/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketClientTransmitter.h"

#include <QDebug>

#include "../Communication/SocketClient.h"

namespace xTools {

SocketClientTransmitter::SocketClientTransmitter(QObject *parent)
    : AbstractTransmitter{parent}
{}

int SocketClientTransmitter::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant SocketClientTransmitter::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto tool = qobject_cast<SocketClient *>(m_tools.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKeys keys;
#if 0
    if (key == keys.enable) {
        return tool->isEnable();
    } else if (key == keys.clientIp) {
        return tool->clientIp();
    } else if (key == keys.clientPort) {
        return tool->clientPort();
    } else if (key == keys.specifiedClientIpPort) {
        return tool->specifyClientIpPort();
    } else if (key == keys.serverIp) {
        return tool->serverIp();
    } else if (key == keys.serverPort) {
        return tool->serverPort();
    } else if (key == keys.messageType) {
        return tool->messageType();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow index");
        qWarning() << "unknown index:" << index;
        return false;
    }
#endif

    return true;
}

bool SocketClientTransmitter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    
    auto baseTool = m_tools.value(index.row());
    auto tool = qobject_cast<SocketClient *>(baseTool);
    QString key = headerData(index.column(), Qt::Horizontal).toString();
#if 0
    ItemContextKeys keys;
    if (key == keys.enable) {
        tool->setIsEnable(value.toBool());
    } else if (key == keys.clientIp) {
        tool->setClientIp(value.toString());
    } else if (key == keys.clientPort) {
        tool->setClientPort(value.toInt());
    } else if (key == keys.specifiedClientIpPort) {
        tool->setSpecifyClientIpPort(value.toBool());
    } else if (key == keys.serverPort) {
        tool->setServerPort(value.toInt());
    } else if (key == keys.serverIp) {
        tool->setServerIp(value.toString());
    } else if (key == keys.messageType) {
        tool->setMessageType(value.toInt());
    } else {
        qWarning() << "unknow key:" << key;
        return false;
    }
#endif

    return true;
}

QVariant SocketClientTransmitter::headerData(int section,
                                             Qt::Orientation orientation,
                                             int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return "--";
    }

    ItemContextKeys keys;
    if (section == 0) {
        return keys.enable;
    } else if (section == 1) {
        return keys.clientIp;
    } else if (section == 2) {
        return keys.clientPort;
    } else if (section == 3) {
        return keys.specifiedClientIpPort;
    } else if (section == 4) {
        return keys.messageType;
    } else if (section == 5) {
        return keys.serverIp;
    } else if (section == 6) {
        return keys.serverPort;
    }

    return "Unknown";
}

} // namespace xTools
