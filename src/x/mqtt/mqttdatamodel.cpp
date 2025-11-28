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

int MqttDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int MqttDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
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

    if (role == Qt::DisplayRole) {
        MqttMessage *msg = m_data.at(row);
        switch (index.column()) {
        case MQTT_DATA_COLUMN_ID:
            return msg->id;
        case MQTT_DATA_COLUMN_CMD:
            return msg->cmd;
        case MQTT_DATA_COLUMN_QOS:
            return msg->qos;
        case MQTT_DATA_COLUMN_ACK:
            return msg->ack;
        case MQTT_DATA_COLUMN_POPS_START:
            return msg->props_start;
        case MQTT_DATA_COLUMN_PROPS_SIZE:
            return msg->props_size;
        case MQTT_DATA_COLUMN_TOPIC:
            return msg->topic;
        case MQTT_DATA_COLUMN_DATA:
            return QString::fromLatin1(msg->data.toHex(' '));
        default:
            return QVariant();
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (index.column() == MQTT_DATA_COLUMN_DATA) {
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
        switch (section) {
        case MQTT_DATA_COLUMN_ID:
            return tr("ID");
        case MQTT_DATA_COLUMN_CMD:
            return tr("CMD");
        case MQTT_DATA_COLUMN_QOS:
            return tr("QoS");
        case MQTT_DATA_COLUMN_ACK:
            return tr("ACK");
        case MQTT_DATA_COLUMN_POPS_START:
            return tr("Props Start");
        case MQTT_DATA_COLUMN_PROPS_SIZE:
            return tr("Props Size");
        case MQTT_DATA_COLUMN_TOPIC:
            return tr("Topic");
        case MQTT_DATA_COLUMN_DATA:
            return tr("Data");
        default:
            return QVariant();
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
