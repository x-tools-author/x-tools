/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttdatamodel.h"

namespace xMQTT {

MqttDataModel::MqttDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

MqttDataModel::~MqttDataModel() {}

void MqttDataModel::addMessage(std::shared_ptr<MqttMessage> message)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    MqttMessage *msg = new MqttMessage(*message);
    m_data.append(msg);
    endInsertRows();
}

void MqttDataModel::clear()
{
    beginResetModel();
    qDeleteAll(m_data);
    m_data.clear();
    endResetModel();
}

int MqttDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int MqttDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant MqttDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    if (row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    int column = index.column();
    if (role == Qt::DisplayRole) {
        MqttMessage *msg = m_data.at(row);
        if (column == MQTT_DATA_COLUMN_TYPE) {
            return msg->isRx ? tr("Rx") : tr("TX");
        } else if (column == MQTT_DATA_COLUMN_CMD) {
            return mqttCmd2String(msg->cmd);
        } else if (column == MQTT_DATA_COLUMN_SERVER) {
            return QString("%1:%2").arg(msg->serverAddress).arg(msg->serverPort);
        } else if (column == MQTT_DATA_COLUMN_CLIENT) {
            return QString("%1:%2").arg(msg->clientAddress).arg(msg->clientPort);
        } else if (column == MQTT_DATA_COLUMN_DATA) {
            return QString(msg->dgram.toHex(' '));
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == MQTT_DATA_COLUMN_DATA) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool MqttDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_data.insert(row, new MqttMessage());
    }
    endInsertRows();
    return true;
}

QVariant MqttDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QString::number(section + 1);
    }

    if (role == Qt::DisplayRole) {
        if (section == MQTT_DATA_COLUMN_TYPE) {
            return tr("Type");
        } else if (section == MQTT_DATA_COLUMN_CMD) {
            return tr("Command");
        } else if (section == MQTT_DATA_COLUMN_SERVER) {
            return tr("Server");
        } else if (section == MQTT_DATA_COLUMN_CLIENT) {
            return tr("Client");
        } else if (section == MQTT_DATA_COLUMN_DATA) {
            return tr("Data");
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (section == MQTT_DATA_COLUMN_DATA) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

} // namespace xMQTT
