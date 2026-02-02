/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QJsonObject>

#define CO_AP_PAYLOAD_MODEL_COLUMN_DESCRIPTION 0
#define CO_AP_PAYLOAD_MODEL_COLUMN_FORMAT 1
#define CO_AP_PAYLOAD_MODEL_COLUMN_DATA 2

#define CO_AP_PAYLOAD_DATA_ROLE_PAYLOAD Qt::UserRole + 1

namespace xCoAP {

class CoAPPayloadModelPrivate;
class CoAPPayloadModel : public QAbstractTableModel
{
    Q_OBJECT
    CoAPPayloadModelPrivate *d;

public:
    explicit CoAPPayloadModel(QObject *parent = nullptr);
    ~CoAPPayloadModel() override;

    void addPayload(const QJsonObject &context);

public:
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

} // namespace xCoAP
