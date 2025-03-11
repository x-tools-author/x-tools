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
    item.serverAddress = data(index(row, SOCKET_ROW_ADDRESS), Qt::EditRole).toString();
    item.serverPort = data(index(row, SOCKET_ROW_PORT), Qt::EditRole).toInt();

    int channel = data(index(row, SOCKET_ROW_CHANNEL), Qt::EditRole).toInt();
    auto cookedChannel = static_cast<WebSocketDataChannel>(channel);
    item.dataChannel = cookedChannel;

    item.authentication = data(index(row, SOCKET_ROW_AUTHENTICATION), Qt::EditRole).toBool();
    item.username = data(index(row, SOCKET_ROW_USERNAME), Qt::EditRole).toString();
    item.password = data(index(row, SOCKET_ROW_PASSWORD), Qt::EditRole).toString();

    QVariantMap map = saveSocketItem(item);
    map.insert("option", data(index(row, SOCKET_ROW_OPTION), Qt::EditRole).toBool());
    map.insert("description", data(index(row, SOCKET_ROW_DESCRIPTION), Qt::EditRole).toString());

    return map;
}

void SocketTransferModel::loadRow(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= rowCount()) {
        return;
    }

    SocketItem socketItem = loadSocketItem(item);
    setData(index(row, SOCKET_ROW_OPTION), item.value("option").toBool(), Qt::EditRole);
    setData(index(row, SOCKET_ROW_ADDRESS), socketItem.serverAddress, Qt::EditRole);
    setData(index(row, SOCKET_ROW_PORT), socketItem.serverPort, Qt::EditRole);
    setData(index(row, SOCKET_ROW_CHANNEL), static_cast<int>(socketItem.dataChannel), Qt::EditRole);
    setData(index(row, SOCKET_ROW_AUTHENTICATION), socketItem.authentication, Qt::EditRole);
    setData(index(row, SOCKET_ROW_USERNAME), socketItem.username, Qt::EditRole);
    setData(index(row, SOCKET_ROW_PASSWORD), socketItem.password, Qt::EditRole);
    setData(index(row, SOCKET_ROW_DESCRIPTION), item.value("description").toString(), Qt::EditRole);

    emit dataChanged(index(row, 0), index(row, columnCount(QModelIndex())));
}

int SocketTransferModel::columnCount(const QModelIndex &parent) const
{
    return 8;
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
    SocketItem socketItem = loadSocketItem(parameters);

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == SOCKET_ROW_OPTION) {
            return transferTypeName(item.option);
        } else if (column == SOCKET_ROW_ADDRESS) {
            return socketItem.serverAddress;
        } else if (column == SOCKET_ROW_PORT) {
            return QString::number(socketItem.serverPort);
        } else if (column == SOCKET_ROW_CHANNEL) {
            auto dataChannel = socketItem.dataChannel;
            return webSocketDataChannelName(dataChannel);
        } else if (column == SOCKET_ROW_AUTHENTICATION) {
            return socketItem.authentication ? tr("Enable") : tr("Disable");
        } else if (column == SOCKET_ROW_USERNAME) {
            return socketItem.username;
        } else if (column == SOCKET_ROW_PASSWORD) {
            return socketItem.password;
        } else if (column == SOCKET_ROW_DESCRIPTION) {
            return item.description;
        }
    } else if (role == Qt::EditRole) {
        if (column == SOCKET_ROW_OPTION) {
            return item.isEnable;
        } else if (column == SOCKET_ROW_ADDRESS) {
            return socketItem.serverAddress;
        } else if (column == SOCKET_ROW_PORT) {
            return socketItem.serverPort;
        } else if (column == SOCKET_ROW_CHANNEL) {
            return static_cast<int>(socketItem.dataChannel);
        } else if (column == SOCKET_ROW_AUTHENTICATION) {
            return socketItem.authentication;
        } else if (column == SOCKET_ROW_USERNAME) {
            return socketItem.username;
        } else if (column == SOCKET_ROW_PASSWORD) {
            return socketItem.password;
        } else if (column == SOCKET_ROW_DESCRIPTION) {
            return item.description;
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == SOCKET_ROW_DESCRIPTION) {
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

    if (column == SOCKET_ROW_OPTION) {
        item.option = value.toInt();
        m_transfers.replace(row, item);
    } else if (column == SOCKET_ROW_DESCRIPTION) {
        item.description = value.toString();
        m_transfers.replace(row, item);
    } else {
        auto socket = qobject_cast<Socket *>(item.transfer);
        QVariantMap parameters = socket->save();
        SocketItem socketItem = loadSocketItem(parameters);

        if (column == SOCKET_ROW_ADDRESS) {
            socketItem.serverAddress = value.toString();
        } else if (column == SOCKET_ROW_PORT) {
            socketItem.serverPort = value.toInt();
        } else if (column == SOCKET_ROW_CHANNEL) {
            socketItem.dataChannel = static_cast<WebSocketDataChannel>(value.toInt());
        } else if (column == SOCKET_ROW_AUTHENTICATION) {
            socketItem.authentication = value.toBool();
        } else if (column == SOCKET_ROW_USERNAME) {
            socketItem.username = value.toString();
        } else if (column == SOCKET_ROW_PASSWORD) {
            socketItem.password = value.toString();
        }

        parameters = saveSocketItem(socketItem);
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
        if (section == SOCKET_ROW_OPTION) {
            return tr("Transfer Option");
        } else if (section == SOCKET_ROW_ADDRESS) {
            return tr("Server Address");
        } else if (section == SOCKET_ROW_PORT) {
            return tr("Server Port");
        } else if (section == SOCKET_ROW_CHANNEL) {
            return tr("Data Channel");
        } else if (section == SOCKET_ROW_AUTHENTICATION) {
            return tr("Authentication");
        } else if (section == SOCKET_ROW_USERNAME) {
            return tr("Username");
        } else if (section == SOCKET_ROW_PASSWORD) {
            return tr("Password");
        } else if (section == SOCKET_ROW_DESCRIPTION) {
            return tr("Description");
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (section == SOCKET_ROW_DESCRIPTION) {
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
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
        socket->load(cookedItem);
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
        if (column == SOCKET_ROW_CHANNEL) {
            // data channel changed
            auto transfer = dynamic_cast<Socket *>(m_transfers.at(row).transfer);
            transfer->setDataChannel(topLeft.data(Qt::EditRole).toInt());
        }
    }
}
