/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tablemodel.h"

#include <QSize>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariantMap TableModel::saveRow(int row)
{
    Q_UNUSED(row);
    QVariantMap data;
    return data;
}

void TableModel::loadRow(int row, const QVariantMap &data)
{
    Q_UNUSED(row);
    Q_UNUSED(data);
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        return QStringLiteral("%1").arg(section + 1);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}