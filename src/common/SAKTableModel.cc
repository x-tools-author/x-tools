/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKTableModel.hh"

SAKTableModel::SAKTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int SAKTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = 0;
    emit invokeGetRowCount(count);
    return count;
}

int SAKTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int column = 0;
    emit invokeGetColumnCount(column);
    return column;
}

QVariant SAKTableModel::data(const QModelIndex &index, int role) const
{
    QVariant d;
    emit invokeGetData(d, index, role);
    return d;
}

bool SAKTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    emit invokeSetData(result, index, value, role);
    return result;
}

bool SAKTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool result = false;
    beginInsertRows(parent, row, rowCount() + count);
    emit invokeInsertRows(result, row, count, parent);
    endInsertRows();
    return result;
}

bool SAKTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool result = false;
    beginRemoveRows(parent, row, row + count - 1);
    emit invokeRemoveRows(result, row, count, parent);
    endRemoveRows();
    return result;
}

QVariant SAKTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant d;
    emit invokeGetHeaderData(d, section, orientation, role);
    return d;
}
