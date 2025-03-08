/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sockettransfermodel.h"

#include "common/xtools.h"
#include "device/socket.h"

SocketTransferModel::SocketTransferModel(QObject *parent)
    : TransferModel(parent)
{}

SocketTransferModel::~SocketTransferModel() {}

QVariantMap SocketTransferModel::saveRow(const int row)
{
    if (row < 0 || row >= rowCount()) {
        return {};
    }

    SocketItem item;
    item.clientAddress = data(index(row, 1), Qt::EditRole).toString();
    item.clientPort = data(index(row, 2), Qt::EditRole).toInt();
    item.serverAddress = data(index(row, 3), Qt::EditRole).toString();
    item.serverPort = data(index(row, 4), Qt::EditRole).toInt();

    int channel = data(index(row, 5), Qt::EditRole).toInt();
    auto cookedChannel = static_cast<WebSocketDataChannel>(channel);
    item.dataChannel = cookedChannel;

    item.authentication = data(index(row, 6), Qt::EditRole).toBool();
    item.username = data(index(row, 7), Qt::EditRole).toString();
    item.password = data(index(row, 8), Qt::EditRole).toString();

    QJsonObject obj = saveSocketItem(item);
    obj.insert("enable", data(index(row, 0), Qt::EditRole).toBool());
    obj.insert("description", data(index(row, 9), Qt::EditRole).toString());

    return obj.toVariantMap();
}

void SocketTransferModel::loadRow(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= rowCount()) {
        return;
    }

    SocketItem socketItem = loadSocketItem(QJsonObject::fromVariantMap(item));
    setData(index(row, 0), item.value("enable").toBool(), Qt::EditRole);
    setData(index(row, 1), socketItem.clientAddress, Qt::EditRole);
    setData(index(row, 2), socketItem.clientPort, Qt::EditRole);
    setData(index(row, 3), socketItem.serverAddress, Qt::EditRole);
    setData(index(row, 4), socketItem.serverPort, Qt::EditRole);
    setData(index(row, 5), static_cast<int>(socketItem.dataChannel), Qt::EditRole);
    setData(index(row, 6), socketItem.authentication, Qt::EditRole);
    setData(index(row, 7), socketItem.username, Qt::EditRole);
    setData(index(row, 8), socketItem.password, Qt::EditRole);
    setData(index(row, 9), item.value("description").toString(), Qt::EditRole);

    emit dataChanged(index(row, 0), index(row, 9));
}

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

    QVariantMap parameters = socket->save();
    SocketItem socketItem = loadSocketItem(QJsonObject::fromVariantMap(parameters));

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return transferTypeName(item.option);
        } else if (column == 1) {
            return socketItem.clientAddress;
        } else if (column == 2) {
            return QString::number(socketItem.clientPort);
        } else if (column == 3) {
            return socketItem.serverAddress;
        } else if (column == 4) {
            return QString::number(socketItem.serverPort);
        } else if (column == 5) {
            auto dataChannel = socketItem.dataChannel;
            return webSocketDataChannelName(dataChannel);
        } else if (column == 6) {
            return socketItem.authentication ? tr("Enable") : tr("Disable");
        } else if (column == 7) {
            return socketItem.username;
        } else if (column == 8) {
            return socketItem.password;
        } else if (column == 9) {
            return item.description;
        }
    } else if (role == Qt::EditRole) {
        if (column == 0) {
            return item.isEnable;
        } else if (column == 1) {
            return socketItem.clientAddress;
        } else if (column == 2) {
            return QString::number(socketItem.clientPort);
        } else if (column == 3) {
            return socketItem.serverAddress;
        } else if (column == 4) {
            return QString::number(socketItem.serverPort);
        } else if (column == 5) {
            return static_cast<int>(socketItem.dataChannel);
        } else if (column == 6) {
            return socketItem.authentication;
        } else if (column == 7) {
            return socketItem.username;
        } else if (column == 8) {
            return socketItem.password;
        } else if (column == 9) {
            return item.description;
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == 9) {
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool SocketTransferModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    int row = index.row();
    if (row < 0 && row >= m_transfers.size()) {
        return false;
    }

    int column = index.column();
    Item item = m_transfers.at(row);

    if (column == 0) {
        item.option = value.toInt();
        m_transfers.replace(row, item);
    } else if (column == 9) {
        item.description = value.toString();
        m_transfers.replace(row, item);
    } else {
        auto socket = qobject_cast<Socket *>(item.transfer);
        QVariantMap parameters = socket->save();
        SocketItem socketItem = loadSocketItem(QJsonObject::fromVariantMap(parameters));

        if (column == 1) {
            socketItem.clientAddress = value.toString();
        } else if (column == 2) {
            socketItem.clientPort = value.toInt();
        } else if (column == 3) {
            socketItem.serverAddress = value.toString();
        } else if (column == 4) {
            socketItem.serverPort = value.toInt();
        } else if (column == 5) {
            socketItem.dataChannel = static_cast<WebSocketDataChannel>(value.toInt());
        } else if (column == 6) {
            socketItem.authentication = value.toBool();
        } else if (column == 7) {
            socketItem.username = value.toString();
        } else if (column == 8) {
            socketItem.password = value.toString();
        }

        parameters = saveSocketItem(socketItem).toVariantMap();
        socket->load(parameters);
    }

    emit dataChanged(index, index);
    return true;
}

QVariant SocketTransferModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Transfer Option");
        } else if (section == 1) {
            return tr("Client Address");
        } else if (section == 2) {
            return tr("Client Port");
        } else if (section == 3) {
            return tr("Server Address");
        } else if (section == 4) {
            return tr("Server Port");
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
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

Device *SocketTransferModel::createTransfer()
{
    auto socket = createSocket();
    if (socket) {
        auto item = defaultSocketItem();
        auto cookedItem = saveSocketItem(item);
        socket->load(cookedItem.toVariantMap());
    }

    return socket;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void SocketTransferModel::onDataChanged(const QModelIndex &topLeft,
                                        const QModelIndex &bottomRight,
                                        const QList<int> &roles)
#else
void SocketTransferModel::onDataChanged(const QModelIndex &topLeft,
                                        const QModelIndex &bottomRight,
                                        const QVector<int> &roles)
#endif
{
    TransferModel::onDataChanged(topLeft, bottomRight, roles);

    auto row = topLeft.row();
    if (row >= 0 && row < m_transfers.size()) {
        int column = topLeft.column();
        if (column == 5) {
            // data channel changed
            auto transfer = dynamic_cast<Socket *>(m_transfers.at(row).transfer);
            transfer->setDataChannel(topLeft.data(Qt::EditRole).toInt());
        }
    }
}
