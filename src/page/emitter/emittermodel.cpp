/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "emittermodel.h"

#include <QDebug>

class EmitterModelPrivate : public QObject
{
public:
    EmitterModelPrivate(EmitterModel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~EmitterModelPrivate() {}

public:
    struct Item
    {
        bool enable{true};
        QString description{"Demo"};
        int interval{1000};
        TextItem textItem;

        int elapsedTime{0};
    };
    struct ItemKeys
    {
        const QString enable{"enable"};
        const QString description{"description"};
        const QString interval{"interval"};
        QString textItem{"textItem"};
    };

public:
    QList<Item> m_items;

private:
    EmitterModel *q;
};

EmitterModel::EmitterModel(QObject *parent)
    : TableModel{parent}
{
    d = new EmitterModelPrivate(this);
}

QVariantMap EmitterModel::saveRow(const int row)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = data(index(row, EMITTER_MODEL_COLUMN_ENABLE), Qt::EditRole);
    bool enable = var.toBool();

    var = data(index(row, EMITTER_MODEL_COLUMN_DESCRIPTION), Qt::DisplayRole);
    QString description = var.toString();

    var = data(index(row, EMITTER_MODEL_COLUMN_INTERVAL), Qt::EditRole);
    int interval = var.toInt();

    var = data(index(row, EMITTER_MODEL_COLUMN_DATA), Qt::EditRole);
    QJsonObject item = var.toJsonObject();

    EmitterModelPrivate::ItemKeys keys;
    QVariantMap map;
    map.insert(keys.textItem, item.toVariantMap());
    map.insert(keys.description, description);
    map.insert(keys.interval, interval);
    map.insert(keys.enable, enable);
    return map;
}

void EmitterModel::loadRow(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    EmitterModelPrivate::ItemKeys keys;
    bool enable = item.value(keys.enable).toBool();
    setData(index(row, EMITTER_MODEL_COLUMN_ENABLE), enable, Qt::EditRole);

    QString description = item.value(keys.description).toString();
    setData(index(row, EMITTER_MODEL_COLUMN_DESCRIPTION), description, Qt::EditRole);

    int interval = item.value(keys.interval).toInt();
    setData(index(row, EMITTER_MODEL_COLUMN_INTERVAL), interval, Qt::EditRole);

    QJsonObject json = item.value(keys.textItem).toJsonObject();
    setData(index(row, EMITTER_MODEL_COLUMN_DATA), json, Qt::EditRole);
}

int EmitterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int count = d->m_items.count();
    return count;
}

int EmitterModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant EmitterModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= d->m_items.count()) {
        return QVariant();
    }

    const EmitterModelPrivate::Item item = d->m_items.at(row);

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == EMITTER_MODEL_COLUMN_ENABLE) {
            return item.enable ? tr("Enable") : tr("Disable");
        } else if (column == EMITTER_MODEL_COLUMN_DESCRIPTION) {
            return item.description;
        } else if (column == EMITTER_MODEL_COLUMN_INTERVAL) {
            return item.interval;
        } else if (column == EMITTER_MODEL_COLUMN_DATA) {
            return xTextItem2string(item.textItem);
        }
    } else if (role == Qt::EditRole) {
        if (column == EMITTER_MODEL_COLUMN_ENABLE) {
            return item.enable;
        } else if (column == EMITTER_MODEL_COLUMN_DESCRIPTION) {
            return item.description;
        } else if (column == EMITTER_MODEL_COLUMN_INTERVAL) {
            return item.interval;
        } else if (column == EMITTER_MODEL_COLUMN_DATA) {
            return xSaveTextItem(item.textItem);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == EMITTER_MODEL_COLUMN_ENABLE || column == EMITTER_MODEL_COLUMN_DESCRIPTION
            || column == EMITTER_MODEL_COLUMN_INTERVAL) {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool EmitterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if (row < 0 || row >= d->m_items.count()) {
        return false;
    }

    bool result = true;
    EmitterModelPrivate::Item &item = d->m_items[row];

    int column = index.column();
    if (role == Qt::EditRole) {
        if (column == EMITTER_MODEL_COLUMN_ENABLE) {
            item.enable = value.toBool();
        } else if (column == EMITTER_MODEL_COLUMN_DESCRIPTION) {
            item.description = value.toString();
        } else if (column == EMITTER_MODEL_COLUMN_INTERVAL) {
            item.interval = value.toInt();
            item.interval = qMax(100, item.interval);
        } else if (column == EMITTER_MODEL_COLUMN_DATA) {
            item.textItem = xLoadTextItem(value.toJsonObject());
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

    TextItem textContext = xDefaultTextItem();
    for (int i = 0; i < count; i++) {
        EmitterModelPrivate::Item item;
        item.enable = true;
        item.description = tr("Demo") + QString::number(rowCount(QModelIndex()));
        item.interval = 1000;
        item.textItem = textContext;
        item.elapsedTime = 0;
        d->m_items.insert(row, item);
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
    for (int i = 0; i < count; i++) {
        d->m_items.removeAt(row);
    }
    endRemoveRows();
    return true;
}

QVariant EmitterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == EMITTER_MODEL_COLUMN_ENABLE) {
            return tr("Enable");
        } else if (section == EMITTER_MODEL_COLUMN_DESCRIPTION) {
            return tr("Description");
        } else if (section == EMITTER_MODEL_COLUMN_INTERVAL) {
            return tr("Interval");
        } else if (section == EMITTER_MODEL_COLUMN_DATA) {
            return tr("Data");
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (section == EMITTER_MODEL_COLUMN_DATA) {
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

Qt::ItemFlags EmitterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fs = QAbstractTableModel::flags(index);
    if (index.column() == EMITTER_MODEL_COLUMN_ENABLE) {
        fs |= Qt::ItemIsEditable;
        fs |= Qt::ItemIsUserCheckable;
    } else if (index.column() == EMITTER_MODEL_COLUMN_DESCRIPTION
               || index.column() == EMITTER_MODEL_COLUMN_INTERVAL) {
        fs |= Qt::ItemIsEditable;
    }

    return fs;
}

void EmitterModel::increaseElapsedTime(const int row, const int interval)
{
    if (row >= 0 || row < d->m_items.count()) {
        d->m_items[row].elapsedTime += interval;
    }
}

bool EmitterModel::isTimeout(const int row) const
{
    bool timeout = false;
    if (row >= 0 || row < d->m_items.count()) {
        timeout = d->m_items[row].elapsedTime >= d->m_items[row].interval;
    }

    return timeout;
}

void EmitterModel::resetElapsedTime(const int row)
{
    if (row >= 0 || row < d->m_items.count()) {
        d->m_items[row].elapsedTime = 0;
    }
}
