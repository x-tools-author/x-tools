/***************************************************************************************************
 * Copyright 2024-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "respondermodel.h"

#include <QDebug>
#include <QStandardItem>

class ResponderModelPrivate : public QObject
{
public:
    ResponderModelPrivate(ResponderModel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}

public:
    struct Item
    {
        bool enable{true};
        QString description{"Demo"};
        ResponseOption option = ResponseOption::InputEqualReference;
        int delay{1000};
        TextItem referenceTextContext;
        TextItem responseTextContext;
    };
    struct ItemKeys
    {
        QString enable{"enable"};
        QString description{"description"};
        QString option{"option"};
        QString delay{"delay"};
        QString reference{"reference"};
        QString response{"response"};
    };

public:
    QList<Item> m_items;

private:
    ResponderModel *q;
};

ResponderModel::ResponderModel(QObject *parent)
    : TableModel{parent}
{
    d = new ResponderModelPrivate(this);
}

QVariantMap ResponderModel::saveRow(const int row)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = data(index(row, RESPONSE_MODEL_COLUMN_ENABLE), Qt::DisplayRole);
    bool enable = var.toBool();

    var = data(index(row, RESPONSE_MODEL_COLUMN_DESCRIPTION), Qt::DisplayRole);
    QString description = var.toString();

    var = data(index(row, RESPONSE_MODEL_COLUMN_OPTION), Qt::EditRole);
    int option = var.toInt();

    var = data(index(row, RESPONSE_MODEL_COLUMN_DELAY), Qt::EditRole);
    int delay = var.toInt();

    var = data(index(row, RESPONSE_MODEL_COLUMN_REFERENCE), Qt::EditRole);
    QJsonObject referenceItem = var.toJsonObject();

    var = data(index(row, RESPONSE_MODEL_COLUMN_RESPONSE), Qt::EditRole);
    QJsonObject responseItem = var.toJsonObject();

    QVariantMap map;
    ResponderModelPrivate::ItemKeys keys;
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

    ResponderModelPrivate::ItemKeys keys;
    bool enable = item.value(keys.enable).toBool();
    setData(index(row, RESPONSE_MODEL_COLUMN_ENABLE), enable, Qt::EditRole);

    QString description = item.value(keys.description).toString();
    setData(index(row, RESPONSE_MODEL_COLUMN_DESCRIPTION), description, Qt::EditRole);

    int option = item.value(keys.option).toInt();
    setData(index(row, RESPONSE_MODEL_COLUMN_OPTION), option, Qt::EditRole);

    int interval = item.value(keys.delay).toInt();
    setData(index(row, RESPONSE_MODEL_COLUMN_DELAY), interval, Qt::EditRole);

    QJsonObject json = item.value(keys.reference).toJsonObject();
    setData(index(row, RESPONSE_MODEL_COLUMN_REFERENCE), json, Qt::EditRole);

    json = item.value(keys.response).toJsonObject();
    setData(index(row, RESPONSE_MODEL_COLUMN_RESPONSE), json, Qt::EditRole);
}

int ResponderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = d->m_items.count();
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
    if (row < 0 || row >= d->m_items.count()) {
        return QVariant();
    }

    const ResponderModelPrivate::Item item = d->m_items.at(row);

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == RESPONSE_MODEL_COLUMN_ENABLE) {
            return item.enable ? tr("Enable") : tr("Disable");
        } else if (column == RESPONSE_MODEL_COLUMN_DESCRIPTION) {
            return item.description;
        } else if (column == RESPONSE_MODEL_COLUMN_OPTION) {
            return xResponseOptionName(item.option);
        } else if (column == RESPONSE_MODEL_COLUMN_DELAY) {
            return item.delay;
        } else if (column == RESPONSE_MODEL_COLUMN_REFERENCE) {
            return xTextItem2string(item.referenceTextContext);
        } else if (column == RESPONSE_MODEL_COLUMN_RESPONSE) {
            return xTextItem2string(item.responseTextContext);
        }
    } else if (role == Qt::EditRole) {
        if (column == RESPONSE_MODEL_COLUMN_ENABLE) {
            return item.enable;
        } else if (column == RESPONSE_MODEL_COLUMN_DESCRIPTION) {
            return item.description;
        } else if (column == RESPONSE_MODEL_COLUMN_OPTION) {
            return static_cast<int>(item.option);
        } else if (column == RESPONSE_MODEL_COLUMN_DELAY) {
            return item.delay;
        } else if (column == RESPONSE_MODEL_COLUMN_REFERENCE) {
            return xSaveTextItem(item.referenceTextContext);
        } else if (column == RESPONSE_MODEL_COLUMN_RESPONSE) {
            return xSaveTextItem(item.responseTextContext);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == RESPONSE_MODEL_COLUMN_ENABLE || column == RESPONSE_MODEL_COLUMN_DESCRIPTION
            || column == RESPONSE_MODEL_COLUMN_OPTION || column == RESPONSE_MODEL_COLUMN_DELAY) {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool ResponderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if (row < 0 || row >= d->m_items.count()) {
        return false;
    }

    bool result = true;
    ResponderModelPrivate::Item &item = d->m_items[row];

    int column = index.column();
    if (role == Qt::EditRole) {
        if (column == RESPONSE_MODEL_COLUMN_ENABLE) {
            item.enable = value.toBool();
        } else if (column == RESPONSE_MODEL_COLUMN_DESCRIPTION) {
            item.description = value.toString();
        } else if (column == RESPONSE_MODEL_COLUMN_OPTION) {
            item.option = static_cast<ResponseOption>(value.toInt());
        } else if (column == RESPONSE_MODEL_COLUMN_DELAY) {
            item.delay = value.toInt();
            item.delay = qMax(0, item.delay);
        } else if (column == RESPONSE_MODEL_COLUMN_REFERENCE) {
            item.referenceTextContext = xLoadTextItem(value.toJsonObject());
        } else if (column == RESPONSE_MODEL_COLUMN_RESPONSE) {
            item.responseTextContext = xLoadTextItem(value.toJsonObject());
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

    TextItem referenceTextContext = xDefaultTextItem();
    TextItem responseTextContext = xDefaultTextItem();
    auto option = ResponseOption::InputEqualReference;
    for (int i = 0; i < count; i++) {
        ResponderModelPrivate::Item item;
        item.enable = true;
        item.description = tr("Demo") + QString::number(rowCount(QModelIndex()));
        item.option = option;
        item.delay = 1000;
        item.referenceTextContext = referenceTextContext;
        item.responseTextContext = responseTextContext;
        d->m_items.insert(row, item);
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
        if (row >= d->m_items.count()) {
            break;
        }
        d->m_items.removeAt(row);
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
        if (section == RESPONSE_MODEL_COLUMN_ENABLE) {
            return tr("Enable");
        } else if (section == RESPONSE_MODEL_COLUMN_DESCRIPTION) {
            return tr("Description");
        } else if (section == RESPONSE_MODEL_COLUMN_OPTION) {
            return tr("Option");
        } else if (section == RESPONSE_MODEL_COLUMN_DELAY) {
            return tr("Delay");
        } else if (section == RESPONSE_MODEL_COLUMN_REFERENCE) {
            return tr("Reference Data");
        } else if (section == RESPONSE_MODEL_COLUMN_RESPONSE) {
            return tr("Response Data");
        }
    }

    return QVariant();
}

Qt::ItemFlags ResponderModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fs = QAbstractTableModel::flags(index);
    int column = index.column();
    if (column == RESPONSE_MODEL_COLUMN_ENABLE) {
        fs |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    } else if (column == RESPONSE_MODEL_COLUMN_DESCRIPTION || column == RESPONSE_MODEL_COLUMN_OPTION
               || column == RESPONSE_MODEL_COLUMN_DELAY) {
        fs |= Qt::ItemIsEditable;
    }

    return fs;
}
