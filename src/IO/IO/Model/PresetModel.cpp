/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PresetModel.h"

namespace xTools {

PresetModel::PresetModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int PresetModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

int PresetModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant PresetModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count()) {
        qWarning() << "Invalid index row: " << index.row();
        return QVariant();
    }

    xIO::TextItem textContext = m_items.at(index.row()).textContext;
    QJsonObject json = xIO::saveTextItem(textContext);

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return m_items.at(index.row()).description;
        } else if (column == 1) {
            return xIO::textItem2string(textContext);
        }
    } else if (role == Qt::EditRole) {
        if (column == 0) {
            return m_items.at(index.row()).description;
        } else if (column == 1) {
            return QVariant::fromValue(json);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == 0) {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool PresetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= m_items.count()) {
        qWarning() << "Invalid index row: " << index.row();
        return false;
    }

    if (index.column() == 0 && role == Qt::EditRole) {
        m_items[index.row()].description = value.toString();
    } else if (index.column() == 1 && role == Qt::EditRole) {
        auto textContext = xIO::loadTextItem(value.toJsonObject());
        m_items[index.row()].textContext = textContext;
    } else {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

bool PresetModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    xIO::TextItem textContext = xIO::defaultTextItem();
    for (int i = 0; i < count; i++) {
        Item item{tr("Demo") + QString::number(rowCount(QModelIndex())), textContext};
        m_items.insert(row, item);
    }

    endInsertRows();
    return true;
}

bool PresetModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count == 0) {
        return true;
    }

    beginRemoveRows(parent, row, row + count - 1);
    m_items.remove(row, count);
    endRemoveRows();
    return true;
}

QVariant PresetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Description");
        } else if (section == 1) {
            return tr("Data");
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (section == 1) {
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

Qt::ItemFlags PresetModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    } else {
        return QAbstractTableModel::flags(index);
    }
}

} // namespace xTools
