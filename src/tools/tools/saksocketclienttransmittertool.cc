/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saksocketclienttransmittertool.h"
#include "saksocketclienttool.h"

SAKSocketClientTransmitterTool::SAKSocketClientTransmitterTool(QObject *parent)
    : SAKTransmitterTool{parent}
{}

QString SAKSocketClientTransmitterTool::cookHeaderString(const QString &str)
{
    ItemContextKeys keys;
    if (str == keys.enable) {
        return tr("Enable");
    } else if (str == keys.clientIp) {
        return tr("ClientIp");
    } else if (str == keys.clientPort) {
        return tr("ClientPort");
    } else if (str == keys.specifiedClientIpPort) {
        return tr("SpecifiedClientIpAndPort");
    } else if (str == keys.serverIp) {
        return tr("ServerIp");
    } else if (str == keys.serverPort) {
        return tr("ServerPort");
    } else if (str == keys.messageType) {
        return tr("MessageType");
    }

    return "--";
}

QVariant SAKSocketClientTransmitterTool::itemContext(int index)
{
    QJsonObject obj;
    ItemContextKeys keys;
    if (index >= 0 && index < mToolVector.count()) {
        SAKSocketClientTool *tool = qobject_cast<SAKSocketClientTool *>(mToolVector.value(index));
        obj.insert(keys.enable, tool->enable());
        obj.insert(keys.clientIp, tool->clientIp());
        obj.insert(keys.clientPort, tool->clientPort());
        obj.insert(keys.specifiedClientIpPort, tool->specifyClientIpPort());
        obj.insert(keys.serverIp, tool->serverIp());
        obj.insert(keys.serverPort, tool->serverPort());
        obj.insert(keys.messageType, tool->messageType());
    } else {
        obj.insert(keys.enable, true);
        obj.insert(keys.clientIp, "127.0.0.1");
        obj.insert(keys.clientPort, 55555);
        obj.insert(keys.specifiedClientIpPort, false);
        obj.insert(keys.serverIp, "127.0.0.1");
        obj.insert(keys.serverPort, 44444);
        obj.insert(keys.messageType, 0);
    }

    return obj;
}

int SAKSocketClientTransmitterTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant SAKSocketClientTransmitterTool::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto tool = qobject_cast<SAKSocketClientTool *>(mToolVector.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKeys keys;
    if (key == keys.enable) {
        return tool->enable();
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

    return true;
}

bool SAKSocketClientTransmitterTool::setData(const QModelIndex &index,
                                             const QVariant &value,
                                             int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    auto baseTool = mToolVector.value(index.row());
    auto tool = qobject_cast<SAKSocketClientTool *>(baseTool);
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKeys keys;
    if (key == keys.enable) {
        tool->setEnable(value.toBool());
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

    return true;
}

QVariant SAKSocketClientTransmitterTool::headerData(int section,
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
