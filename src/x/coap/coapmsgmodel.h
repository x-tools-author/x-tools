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

#include "coapcommon.h"

#define CO_AP_MODEL_COLUMN_FLAG 0
#define CO_AP_MODEL_COLUMN_MESSAGE_ID 1
#define CO_AP_MODEL_COLUMN_HOST_LOCAL 2
#define CO_AP_MODEL_COLUMN_HOST_REMOTE 3
#define CO_AP_MODEL_COLUMN_VERSION 4
#define CO_AP_MODEL_COLUMN_TYPE 5
#define CO_AP_MODEL_COLUMN_TOKEN_LENGTH 6
#define CO_AP_MODEL_COLUMN_TOKEN 7
#define CO_AP_MODEL_COLUMN_OPTIONS 8
#define CO_AP_MODEL_COLUMN_PAYLOAD 9

namespace xCoAP {
class CoAPMsgModelPrivate;
class CoAPMsgModel : public QAbstractTableModel
{
    Q_OBJECT
    CoAPMsgModelPrivate *d;

public:
    explicit CoAPMsgModel(QObject *parent = nullptr);
    ~CoAPMsgModel() override;
    void addRow(std::shared_ptr<CoAPMsgItem> request, std::shared_ptr<CoAPMsgItem> response);
    void clear();

public:
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

} // namespace xCoAP
