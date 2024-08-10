/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "SocketTransferModel.h"
#include "../../xIO.h"
#include "../Communication/Socket.h"

namespace xTools {

SocketTransferModel::SocketTransferModel(QObject *parent)
    : AbstractTransferModel(parent)
{}

SocketTransferModel::~SocketTransferModel() {}

int SocketTransferModel::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QVariant SocketTransferModel::data(const QModelIndex &index, int role) const
{
    int rows = rowCount(QModelIndex());
    if (rows < 0 || index.row() >= rows) {
        return QVariant();
    }

    Item item = m_transfers.at(index.row());
    auto socket = qobject_cast<Socket *>(item.transfer);
    if (!socket) {
        return QVariant();
    }

    QVariantMap parameters = socket->parameters();
    xIO::SocketItem socketItem = xIO::loadSocketItem(QJsonObject::fromVariantMap(parameters));

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return item.enabled ? tr("Enable") : tr("Disable");
        } else if (index.column() == 1) {
            return socketItem.clientAddress;
        } else if (index.column() == 2) {
            return QString::number(socketItem.clientPort);
        } else if (index.column() == 3) {
            return socketItem.serverAddress;
        } else if (index.column() == 4) {
            return QString::number(socketItem.serverPort);
        } else if (index.column() == 5) {
            auto dataChannel = socketItem.dataChannel;
            return xIO::webSocketDataChannelName(dataChannel);
        } else if (index.column() == 6) {
            return socketItem.authentication ? tr("Enable") : tr("Disable");
        } else if (index.column() == 7) {
            return socketItem.username;
        } else if (index.column() == 8) {
            return socketItem.password;
        } else if (index.column() == 9) {
            return item.description;
        }
    } else if (role == Qt::EditRole) {
        if (index.column() == 0) {
            return item.enabled;
        } else if (index.column() == 1) {
            return socketItem.clientAddress;
        } else if (index.column() == 2) {
            return QString::number(socketItem.clientPort);
        } else if (index.column() == 3) {
            return socketItem.serverAddress;
        } else if (index.column() == 4) {
            return QString::number(socketItem.serverPort);
        } else if (index.column() == 5) {
            return static_cast<int>(socketItem.dataChannel);
        } else if (index.column() == 6) {
            return socketItem.authentication;
        } else if (index.column() == 7) {
            return socketItem.username;
        } else if (index.column() == 8) {
            return socketItem.password;
        } else if (index.column() == 9) {
            return item.description;
        }
    }

    return QVariant();
}

bool SocketTransferModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

QVariant SocketTransferModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Enable");
        } else if (section == 1) {
            return tr("Client Port");
        } else if (section == 2) {
            return tr("Client Address");
        } else if (section == 3) {
            return tr("Server Port");
        } else if (section == 4) {
            return tr("Server Address");
        } else if (section == 5) {
            return tr("Data Channel");
        } else if (section == 6) {
            return tr("Authentication");
        } else if (section == 7) {
            return tr("Username");
        } else if (section == 8) {
            return tr("Password");
        } else if (section == 9) {
            return tr("Description");
        }
    }

    return QVariant();
}

Qt::ItemFlags SocketTransferModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

Communication *SocketTransferModel::createTransfer()
{
    auto socket = createSocket();
    if (socket) {
        auto item = xIO::defaultSocketItem();
        auto cookedItem = xIO::saveSocketItem(item);
        socket->setParameters(cookedItem.toVariantMap());
    }

    return socket;
}

} // namespace xTools
