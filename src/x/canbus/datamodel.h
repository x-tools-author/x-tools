/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#define X_CAN_BUS_COLUMN_TIME 0
#define X_CAN_BUS_COLUMN_TYPE 1
#define X_CAN_BUS_COLUMN_FLAGS 2
#define X_CAN_BUS_COLUMN_ID 3
#define X_CAN_BUS_COLUMN_DLC 4
#define X_CAN_BUS_COLUMN_DATA 5

namespace xCanBus {

class DataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);
    ~DataModel() override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

} // namespace xCanBus