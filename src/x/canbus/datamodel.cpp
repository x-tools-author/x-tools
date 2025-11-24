/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "datamodel.h"

namespace xCanBus {

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

DataModel::~DataModel() {}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 0;
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);
    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case X_CAN_BUS_COLUMN_TIME:
                return tr("Time");
            case X_CAN_BUS_COLUMN_TYPE:
                return tr("Type");
            case X_CAN_BUS_COLUMN_FLAGS:
                return tr("Flags");
            case X_CAN_BUS_COLUMN_ID:
                return tr("ID");
            case X_CAN_BUS_COLUMN_DLC:
                return tr("DLC");
            case X_CAN_BUS_COLUMN_DATA:
                return tr("Payload");
            default:
                qWarning("invalid section %d", section);
                return QVariant();
            }
        } else {
            return QString::number(section + 1);
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            if (section == X_CAN_BUS_COLUMN_DATA) {
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        }

        return Qt::AlignCenter;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

} // namespace xCanBus