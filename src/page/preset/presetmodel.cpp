/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "presetmodel.h"

#include <QDebug>

class PresetModelPrivate : public QObject
{
public:
    PresetModelPrivate(PresetModel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}

public:
    struct Item
    {
        QString description{"Demo"};
        TextItem textContext;
    };
    QList<Item> m_items;

private:
    PresetModel *q;
};

PresetModel::PresetModel(QObject *parent)
    : TableModel{parent}
{
    d = new PresetModelPrivate(this);
}

QVariantMap PresetModel::saveRow(const int row)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = data(index(row, PRESET_MODEL_COLUMN_DESC), Qt::DisplayRole);
    QString description = var.toString();

    var = data(index(row, PRESET_MODEL_COLUMN_DATA), Qt::EditRole);
    QJsonObject item = var.toJsonObject();

    QVariantMap map = item.toVariantMap();
    map.insert("description", description);
    return map;
}

void PresetModel::loadRow(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    QString description = item.value("description").toString();
    setData(index(row, PRESET_MODEL_COLUMN_DESC), description, Qt::EditRole);

    QJsonObject json = QJsonObject::fromVariantMap(item);
    setData(index(row, PRESET_MODEL_COLUMN_DATA), json, Qt::EditRole);
}

int PresetModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->m_items.count();
}

int PresetModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant PresetModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= d->m_items.count()) {
        qWarning() << "Invalid index row: " << index.row();
        return QVariant();
    }

    TextItem textContext = d->m_items.at(index.row()).textContext;
    QJsonObject json = xSaveTextItem(textContext);

    int column = index.column();
    if (role == Qt::DisplayRole) {
        if (column == PRESET_MODEL_COLUMN_SEND) {
            return tr("Send");
        } else if (column == PRESET_MODEL_COLUMN_DESC) {
            return d->m_items.at(index.row()).description;
        } else if (column == PRESET_MODEL_COLUMN_DATA) {
            return xTextItem2string(textContext);
        }
    } else if (role == Qt::EditRole) {
        if (column == PRESET_MODEL_COLUMN_DESC) {
            return d->m_items.at(index.row()).description;
        } else if (column == PRESET_MODEL_COLUMN_DATA) {
            return QVariant::fromValue(json);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == PRESET_MODEL_COLUMN_SEND) {
            return int(Qt::AlignCenter);
        } else {
            return int(int(Qt::AlignLeft) | int(Qt::AlignVCenter));
        }
    }

    return QVariant();
}

bool PresetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= d->m_items.count()) {
        qWarning() << "Invalid index row: " << index.row();
        return false;
    }

    if (index.column() == PRESET_MODEL_COLUMN_DESC && role == Qt::EditRole) {
        d->m_items[index.row()].description = value.toString();
    } else if (index.column() == PRESET_MODEL_COLUMN_DATA && role == Qt::EditRole) {
        auto textContext = xLoadTextItem(value.toJsonObject());
        d->m_items[index.row()].textContext = textContext;
    } else {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

bool PresetModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    TextItem textContext = xDefaultTextItem();
    for (int i = 0; i < count; i++) {
        PresetModelPrivate::Item item;
        item.description = tr("Demo") + QString::number(rowCount(QModelIndex()));
        item.textContext = textContext;
        d->m_items.insert(row, item);
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
    for (int i = 0; i < count; i++) {
        d->m_items.removeAt(row);
    }
    endRemoveRows();
    return true;
}

QVariant PresetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return QVariant(QString::number(section + 1));
    }

    if (role == Qt::DisplayRole) {
        if (section == PRESET_MODEL_COLUMN_SEND) {
            return tr("Option");
        }
        if (section == PRESET_MODEL_COLUMN_DESC) {
            return tr("Description");
        } else if (section == PRESET_MODEL_COLUMN_DATA) {
            return tr("Data");
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignLeft;
    }

    return QVariant();
}

Qt::ItemFlags PresetModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fs = QAbstractTableModel::flags(index);
    if (index.column() == PRESET_MODEL_COLUMN_DESC) {
        fs |= Qt::ItemIsEditable;
    }

    return fs;
}

bool PresetModel::moveRows(const QModelIndex &sourceParent,
                           int sourceRow,
                           int count,
                           const QModelIndex &destinationParent,
                           int destinationChild)
{
    if (sourceRow < 0 || sourceRow >= d->m_items.count() || destinationChild < 0
        || destinationChild > d->m_items.count()
        || (sourceRow <= destinationChild && destinationChild < sourceRow + 1)) {
        qWarning() << "Invalid moveRows parameters: "
                   << "sourceRow=" << sourceRow << ", count=" << 1
                   << ", destinationChild=" << destinationChild;
        return false;
    }

    PresetModelPrivate::Item sourceItem = d->m_items.at(sourceRow);
    PresetModelPrivate::Item destinationItem = d->m_items.at(destinationChild);
    d->m_items.replace(destinationChild, sourceItem);
    d->m_items.replace(sourceRow, destinationItem);

    QModelIndex topLeft = index(sourceRow, 0);
    QModelIndex bottomRight = index(sourceRow, columnCount(QModelIndex()) - 1);
    emit dataChanged(topLeft, bottomRight);

    return true;
}
