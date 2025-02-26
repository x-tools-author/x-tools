/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "emittermodel.h"

namespace xTools {

EmitterModel::EmitterModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int EmitterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = m_items.count();
    return count;
}

int EmitterModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant EmitterModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_items.count()) {
        return QVariant();
    }

    const Item item = m_items.at(row);

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return item.enable ? tr("Enable") : tr("Disable");
        } else if (column == 1) {
            return item.description;
        } else if (column == 2) {
            return item.interval;
        } else if (column == 3) {
            return textItem2string(item.textContext);
        }
    } else if (role == Qt::EditRole) {
        if (column == 0) {
            return item.enable;
        } else if (column == 1) {
            return item.description;
        } else if (column == 2) {
            return item.interval;
        } else if (column == 3) {
            return saveTextItem(item.textContext);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == 0 || column == 1 || column == 2) {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool EmitterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if (row < 0 || row >= m_items.count()) {
        return false;
    }

    bool result = true;
    Item &item = m_items[row];

    int column = index.column();
    if (role == Qt::EditRole) {
        if (column == 0) {
            item.enable = value.toBool();
        } else if (column == 1) {
            item.description = value.toString();
        } else if (column == 2) {
            item.interval = value.toInt();
            item.interval = qMax(100, item.interval);
        } else if (column == 3) {
            item.textContext = loadTextItem(value.toJsonObject());
        } else {
            result = false;
        }
    }

    emit dataChanged(index, index);
    return result;
}

bool EmitterModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    TextItem textContext = defaultTextItem();
    for (int i = 0; i < count; i++) {
        Item item{true, tr("Demo") + QString::number(rowCount(QModelIndex())), 1000, textContext};
        m_items.insert(row, item);
    }

    endInsertRows();
    return true;
}

bool EmitterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count == 0) {
        return true;
    }

    beginRemoveRows(parent, row, row + count - 1);
    m_items.remove(row, count);
    endRemoveRows();
    return true;
}

QVariant EmitterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Enable");
        } else if (section == 1) {
            return tr("Description");
        } else if (section == 2) {
            return tr("Interval");
        } else if (section == 3) {
            return tr("Data");
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (section == 3) {
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

Qt::ItemFlags EmitterModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
    } else if (index.column() == 1 || index.column() == 2) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    } else {
        return QAbstractTableModel::flags(index);
    }
}

void EmitterModel::increaseElapsedTime(const int row, const int interval)
{
    if (row >= 0 || row < m_items.count()) {
        m_items[row].elapsedTime += interval;
    }
}

bool EmitterModel::isTimeout(const int row) const
{
    bool timeout = false;
    if (row >= 0 || row < m_items.count()) {
        timeout = m_items[row].elapsedTime >= m_items[row].interval;
    }

    return timeout;
}

void EmitterModel::resetElapsedTime(const int row)
{
    if (row >= 0 || row < m_items.count()) {
        m_items[row].elapsedTime = 0;
    }
}

} // namespace xTools
