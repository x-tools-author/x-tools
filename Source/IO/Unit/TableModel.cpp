/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TableModel.h"

namespace xTools {

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

void TableModel::setEitableColumns(const QList<int> &columns)
{
    m_editableColumns = columns;
}

void TableModel::setCheckableColumns(const QList<int> &columns)
{
    m_checkableColumns = columns;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = 0;
    emit const_cast<TableModel *>(this)->invokeGetRowCount(count);
    return count;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int column = 0;
    emit const_cast<TableModel *>(this)->invokeGetColumnCount(column);
    return column;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    QVariant d;
    emit const_cast<TableModel *>(this)->invokeGetData(d, index, role);
    return d;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    emit invokeSetData(result, index, value, role);
    emit dataChanged(index, index);
    return result;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool result = false;
    beginInsertRows(parent, row, row + count - 1);
    emit invokeInsertRows(result, row, count, parent);
    endInsertRows();
    return result;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
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

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant d;
    emit const_cast<TableModel *>(this)->invokeGetHeaderData(d, section, orientation, role);
    return d;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (m_checkableColumns.contains(index.column())) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    } else if (m_editableColumns.contains(index.column())) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    } else {
        return QAbstractTableModel::flags(index);
    }
}

} // namespace xTools
