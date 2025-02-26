/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "respondermodel.h"

#include <QStandardItem>

namespace xTools {

ResponderModel::ResponderModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int ResponderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = m_items.count();
    return count;
}

int ResponderModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant ResponderModel::data(const QModelIndex &index, int role) const
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
            return xIO::responseOptionName(item.option);
        } else if (column == 3) {
            return item.delay;
        } else if (column == 4) {
            return xIO::textItem2string(item.referenceTextContext);
        } else if (column == 5) {
            return xIO::textItem2string(item.responseTextContext);
        }
    } else if (role == Qt::EditRole) {
        if (column == 0) {
            return item.enable;
        } else if (column == 1) {
            return item.description;
        } else if (column == 2) {
            return static_cast<int>(item.option);
        } else if (column == 3) {
            return item.delay;
        } else if (column == 4) {
            return xIO::saveTextItem(item.referenceTextContext);
        } else if (column == 5) {
            return xIO::saveTextItem(item.responseTextContext);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == 0 || column == 1 || column == 2 || column == 3) {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool ResponderModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
            item.option = static_cast<xIO::ResponseOption>(value.toInt());
        } else if (column == 3) {
            item.delay = value.toInt();
            item.delay = qMax(0, item.delay);
        } else if (column == 4) {
            item.referenceTextContext = xIO::loadTextItem(value.toJsonObject());
        } else if (column == 5) {
            item.responseTextContext = xIO::loadTextItem(value.toJsonObject());
        } else {
            result = false;
        }
    }

    emit dataChanged(index, index);
    return result;
}

bool ResponderModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    xIO::TextItem referenceTextContext = xIO::defaultTextItem();
    xIO::TextItem responseTextContext = xIO::defaultTextItem();
    auto option = xIO::ResponseOption::InputEqualReference;
    for (int i = 0; i < count; i++) {
        Item item{true,
                  tr("Demo") + QString::number(rowCount(QModelIndex())),
                  option,
                  1000,
                  referenceTextContext,
                  responseTextContext};
        m_items.insert(row, item);
    }

    endInsertRows();
    return true;
}

bool ResponderModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count == 0) {
        return true;
    }

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++) {
        if (row >= m_items.count()) {
            break;
        }
        m_items.removeAt(row);
    }
    endRemoveRows();
    return true;
}

QVariant ResponderModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return tr("Option");
        } else if (section == 3) {
            return tr("Delay");
        } else if (section == 4) {
            return tr("Reference Data");
        } else if (section == 5) {
            return tr("Response Data");
        }
    }

    return QVariant();
}

Qt::ItemFlags ResponderModel::flags(const QModelIndex &index) const
{
    int column = index.column();
    if (column == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    } else if (column == 1 || column == 2 || column == 3) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    } else {
        return QAbstractTableModel::flags(index);
    }
}

} // namespace xTools
