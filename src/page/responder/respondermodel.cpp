/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "respondermodel.h"

#include <QDebug>
#include <QStandardItem>

ResponderModel::ResponderModel(QObject *parent)
    : TableModel{parent}
{}

QVariantMap ResponderModel::saveRow(const int row)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = data(index(row, 0), Qt::DisplayRole);
    bool enable = var.toBool();

    var = data(index(row, 1), Qt::DisplayRole);
    QString description = var.toString();

    var = data(index(row, 2), Qt::EditRole);
    int option = var.toInt();

    var = data(index(row, 3), Qt::EditRole);
    int delay = var.toInt();

    var = data(index(row, 4), Qt::EditRole);
    QJsonObject referenceItem = var.toJsonObject();

    var = data(index(row, 5), Qt::EditRole);
    QJsonObject responseItem = var.toJsonObject();

    QVariantMap map;
    ItemKeys keys;
    map.insert(keys.enable, enable);
    map.insert(keys.description, description);
    map.insert(keys.option, option);
    map.insert(keys.delay, delay);
    map.insert(keys.reference, referenceItem);
    map.insert(keys.response, responseItem);
    return map;
}

void ResponderModel::loadRow(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    ItemKeys keys;
    bool enable = item.value(keys.enable).toBool();
    setData(index(row, 0), enable, Qt::EditRole);

    QString description = item.value(keys.description).toString();
    setData(index(row, 1), description, Qt::EditRole);

    int option = item.value(keys.option).toInt();
    setData(index(row, 2), option, Qt::EditRole);

    int interval = item.value(keys.delay).toInt();
    setData(index(row, 3), interval, Qt::EditRole);

    QJsonObject json = item.value(keys.reference).toJsonObject();
    setData(index(row, 4), json, Qt::EditRole);

    json = item.value(keys.response).toJsonObject();
    setData(index(row, 5), json, Qt::EditRole);
}

int ResponderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = m_items.count();
    return count;
}

int ResponderModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
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
            return responseOptionName(item.option);
        } else if (column == 3) {
            return item.delay;
        } else if (column == 4) {
            return textItem2string(item.referenceTextContext);
        } else if (column == 5) {
            return textItem2string(item.responseTextContext);
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
            return saveTextItem(item.referenceTextContext);
        } else if (column == 5) {
            return saveTextItem(item.responseTextContext);
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
            item.option = static_cast<ResponseOption>(value.toInt());
        } else if (column == 3) {
            item.delay = value.toInt();
            item.delay = qMax(0, item.delay);
        } else if (column == 4) {
            item.referenceTextContext = loadTextItem(value.toJsonObject());
        } else if (column == 5) {
            item.responseTextContext = loadTextItem(value.toJsonObject());
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

    TextItem referenceTextContext = defaultTextItem();
    TextItem responseTextContext = defaultTextItem();
    auto option = ResponseOption::InputEqualReference;
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
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    } else if (column == 1 || column == 2 || column == 3) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    } else {
        return QAbstractTableModel::flags(index);
    }
}
