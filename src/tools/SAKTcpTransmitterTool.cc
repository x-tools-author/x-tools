/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKUdpClientTool.hh"
#include "SAKTcpTransmitterTool.hh"

SAKTcpTransmitterTool::SAKTcpTransmitterTool(QObject *parent)
    : SAKTransmitterTool{"SAK.TcpTransmitterTool", parent}
{

}

QVariant SAKTcpTransmitterTool::itemContext(int index)
{
    QJsonObject obj;
    ItemContextKeys keys;
    if (index >= 0 && index < mToolVector.count()) {
        SAKUdpClientTool *tool =
            qobject_cast<SAKUdpClientTool*>(mToolVector.value(index));
        obj.insert(keys.enable, tool->enable());
        obj.insert(keys.clientIp, tool->clientIp());
        obj.insert(keys.clientPort, tool->clientPort());
        obj.insert(keys.specifiedClientIpPort, tool->specifyClientIpPort());
        obj.insert(keys.serverIp, tool->serverIp());
        obj.insert(keys.serverPort, tool->serverPort());
    } else {
        obj.insert(keys.enable, true);
        obj.insert(keys.clientIp, "127.0.0.1");
        obj.insert(keys.clientPort, 55555);
        obj.insert(keys.specifiedClientIpPort, false);
        obj.insert(keys.serverIp, "127.0.0.1");
        obj.insert(keys.serverPort, 44444);
    }

    return obj;
}

int SAKTcpTransmitterTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 6;
}

QVariant SAKTcpTransmitterTool::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto tool = qobject_cast<SAKUdpClientTool*>(mToolVector.value(index.row()));
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
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow index");
        qCWarning(mLoggingCategory) << "unknown index:" << index;
        return false;
    }

    return true;
}

bool SAKTcpTransmitterTool::setData(const QModelIndex &index,
                                    const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    auto baseTool = mToolVector.value(index.row());
    auto tool = qobject_cast<SAKUdpClientTool*>(baseTool);
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
    } else {
        qCWarning(mLoggingCategory) << "unknow key:" << key;
        return false;
    }

    return true;
}

bool SAKTcpTransmitterTool::insertRows(int row, int count,
                                       const QModelIndex &parent)
{
    Q_UNUSED(parent)
    for (int i = 0; i < count; i++) {
        mToolVectorMutex.lock();
        auto tool = new SAKUdpClientTool(this);
        connect(this, &SAKUdpClientTool::bytesInputted,
                tool, &SAKUdpClientTool::inputBytes);
        connect(tool, &SAKUdpClientTool::bytesOutputted,
                this, &SAKUdpClientTool::bytesOutputted);
        connect(this, &SAKUdpClientTool::started,
                tool, [=](){tool->start();});
        connect(this, &SAKUdpClientTool::finished,
                tool, [=](){tool->exit();});

        mToolVector.insert(row, tool);
        mToolVectorMutex.unlock();
    }

    return true;
}

QVariant SAKTcpTransmitterTool::headerData(int section,
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
        return keys.serverIp;
    } else if (section == 5) {
        return keys.serverPort;
    }

    return "Unknown";
}
