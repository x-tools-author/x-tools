/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "registermodel.h"

#define REGISTER_TABLE_ADDRESS 0
#define REGISTER_TABLE_NAME 1
#define REGISTER_TABLE_TYPE 2
#define REGISTER_TABLE_UNIT 3
#define REGISTER_TABLE_DESCRIPTION 4
#define REGISTER_TABLE_MIN 5
#define REGISTER_TABLE_MAX 6
#define REGISTER_TABLE_DECIMALS 7

namespace xModbus {

RegisterModel::RegisterModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

RegisterModel::~RegisterModel() {}

void RegisterModel::addRegisterItem(const RegisterItem &item)
{
    RegisterItem *newItem = new RegisterItem(item);
    m_registerItems.append(newItem);
    insertRows(m_registerItems.count() - 1, 1, QModelIndex());
}

void RegisterModel::addRegisterItems(const QList<RegisterItem> &items)
{
    for (const RegisterItem &item : items) {
        addRegisterItem(item);
    }
}

int RegisterModel::rowCount(const QModelIndex &parent) const
{
    return m_registerItems.count();
}

int RegisterModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant RegisterModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.row() < 0 || index.row() >= m_registerItems.count())
        return QVariant();

    const RegisterItem *item = m_registerItems.at(index.row());
    switch (index.column()) {
    case REGISTER_TABLE_ADDRESS:
        return item->address;
    case REGISTER_TABLE_NAME:
        return item->name;
    case REGISTER_TABLE_TYPE:
        return item->type;
    case REGISTER_TABLE_UNIT:
        return item->unit;
    case REGISTER_TABLE_DESCRIPTION:
        return item->description;
    case REGISTER_TABLE_MIN:
        return item->min;
    case REGISTER_TABLE_MAX:
        return item->max;
    case REGISTER_TABLE_DECIMALS:
        return item->decimals;
    default:
        return QVariant();
    }
}

bool RegisterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    if (index.row() < 0 || index.row() >= m_registerItems.count())
        return false;

    RegisterItem *item = m_registerItems.at(index.row());
    switch (index.column()) {
    case REGISTER_TABLE_ADDRESS:
        item->address = value.toUInt();
        break;
    case REGISTER_TABLE_NAME:
        item->name = value.toString();
        break;
    case REGISTER_TABLE_TYPE:
        item->type = static_cast<QModbusDataUnit::RegisterType>(value.toInt());
        break;
    case REGISTER_TABLE_UNIT:
        item->unit = value.toString();
        break;
    case REGISTER_TABLE_DESCRIPTION:
        item->description = value.toString();
        break;
    case REGISTER_TABLE_MIN:
        item->min = value.toDouble();
        break;
    case REGISTER_TABLE_MAX:
        item->max = value.toDouble();
        break;
    case REGISTER_TABLE_DECIMALS:
        item->decimals = value.toInt();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case REGISTER_TABLE_ADDRESS:
            return tr("Address");
        case REGISTER_TABLE_NAME:
            return tr("Name");
        case REGISTER_TABLE_TYPE:
            return tr("Type");
        case REGISTER_TABLE_UNIT:
            return tr("Unit");
        case REGISTER_TABLE_DESCRIPTION:
            return tr("Description");
        case REGISTER_TABLE_MIN:
            return tr("Min");
        case REGISTER_TABLE_MAX:
            return tr("Max");
        case REGISTER_TABLE_DECIMALS:
            return tr("Decimals");
        default:
            return QVariant();
        }
    } else {
        return section + 1;
    }
}

bool RegisterModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        RegisterItem *newItem = new RegisterItem();
        m_registerItems.insert(row, newItem);
    }
    endInsertRows();

    return true;
}

bool RegisterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        RegisterItem *item = m_registerItems.takeAt(row);
        delete item;
    }
    endRemoveRows();

    return true;
}

} // namespace xModbus