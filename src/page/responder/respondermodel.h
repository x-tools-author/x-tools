/***************************************************************************************************
 * Copyright 2024-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "common/x.h"
#include "page/common/tablemodel.h"

#define RESPONSE_MODEL_COLUMN_ENABLE 0
#define RESPONSE_MODEL_COLUMN_DESCRIPTION 1
#define RESPONSE_MODEL_COLUMN_OPTION 2
#define RESPONSE_MODEL_COLUMN_DELAY 3
#define RESPONSE_MODEL_COLUMN_REFERENCE 4
#define RESPONSE_MODEL_COLUMN_RESPONSE 5

class ResponderModelPrivate;
class ResponderModel : public TableModel
{
    Q_OBJECT
    ResponderModelPrivate *d;

public:
    explicit ResponderModel(QObject *parent = nullptr);

    QVariantMap saveRow(const int row) override;
    void loadRow(const int row, const QVariantMap &item) override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
};
