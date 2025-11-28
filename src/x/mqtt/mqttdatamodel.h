/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include "mqttcommon.h"

#include <QAbstractTableModel>

#define MQTT_DATA_COLUMN_ID 0
#define MQTT_DATA_COLUMN_CMD 1
#define MQTT_DATA_COLUMN_QOS 2
#define MQTT_DATA_COLUMN_ACK 3
#define MQTT_DATA_COLUMN_POPS_START 4
#define MQTT_DATA_COLUMN_PROPS_SIZE 5
#define MQTT_DATA_COLUMN_TOPIC 6
#define MQTT_DATA_COLUMN_DATA 7

namespace xMQTT {

class MqttMessage;
class MqttDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MqttDataModel(QObject *parent = nullptr);
    ~MqttDataModel() override;
    void addMessage(std::shared_ptr<MqttMessage> message);

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<MqttMessage *> m_data;
};

} // namespace xMQTT
