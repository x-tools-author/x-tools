/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTableModel.h"

xToolsTableModel::xToolsTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int xToolsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = 0;
    emit const_cast<xToolsTableModel *>(this)->invokeGetRowCount(count);
    return count;
}

int xToolsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int column = 0;
    emit const_cast<xToolsTableModel *>(this)->invokeGetColumnCount(column);
    return column;
}

QVariant xToolsTableModel::data(const QModelIndex &index, int role) const
{
    QVariant d;
    emit const_cast<xToolsTableModel *>(this)->invokeGetData(d, index, role);
    return d;
}

bool xToolsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    emit invokeSetData(result, index, value, role);
    emit dataChanged(index, index);
    return result;
}

bool xToolsTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool result = false;
    beginInsertRows(parent, row, row + count - 1);
    emit invokeInsertRows(result, row, count, parent);
    endInsertRows();
    return result;
}

bool xToolsTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count == 0) {
        return true;
    }

    bool result = false;
    beginRemoveRows(parent, row, row + count - 1);
    emit invokeRemoveRows(result, row, count, parent);
    endRemoveRows();
    return result;
}

QVariant xToolsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant d;
    emit const_cast<xToolsTableModel *>(this)->invokeGetHeaderData(d, section, orientation, role);
    return d;
}
