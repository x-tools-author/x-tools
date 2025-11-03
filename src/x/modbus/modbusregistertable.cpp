/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregistertable.h"

#define REGISTER_TABLE_ADDRESS 0
#define REGISTER_TABLE_TYPE 1
#define REGISTER_TABLE_MIN 2
#define REGISTER_TABLE_MAX 3
#define REGISTER_TABLE_DECIMALS 4
#define REGISTER_TABLE_NAME 5
#define REGISTER_TABLE_VALUE 6
#define REGISTER_TABLE_UNIT 7
#define REGISTER_TABLE_DESCRIPTION 8

namespace xModbus {

ModbusRegisterTable::ModbusRegisterTable(QObject *parent)
    : QAbstractTableModel(parent)
{}

ModbusRegisterTable::~ModbusRegisterTable() {}

RegisterItem *ModbusRegisterTable::addRegisterItem(const RegisterItem &item)
{
    RegisterItem *newItem = new RegisterItem(item);
    beginInsertRows(QModelIndex(), m_registerItems.count(), m_registerItems.count());
    m_registerItems.append(newItem);
    endInsertRows();
    return newItem;
}

void ModbusRegisterTable::addRegisterItems(const QList<RegisterItem> &items)
{
    for (const RegisterItem &item : items) {
        addRegisterItem(item);
    }
}

int ModbusRegisterTable::rowCount(const QModelIndex &parent) const
{
    return m_registerItems.count();
}

int ModbusRegisterTable::columnCount(const QModelIndex &parent) const
{
    return 9;
}

QVariant ModbusRegisterTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole) {
        if (index.column() == REGISTER_TABLE_DESCRIPTION) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= m_registerItems.count()) {
        return QVariant();
    }

    const RegisterItem *item = m_registerItems.at(index.row());
    switch (index.column()) {
    case REGISTER_TABLE_ADDRESS:
        return QString("0x%1").arg(item->address, 4, 16, QChar('0').toUpper());
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
    case REGISTER_TABLE_VALUE:
        return item->value;
    default:
        return QVariant();
    }
}

bool ModbusRegisterTable::setData(const QModelIndex &index, const QVariant &value, int role)
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
    case REGISTER_TABLE_VALUE:
        item->value = static_cast<qint16>(value.toInt());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

QVariant ModbusRegisterTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
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
            case REGISTER_TABLE_VALUE:
                return tr("Value");
            default:
                return QVariant();
            }
        } else {
            return section + 1;
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            if (section == REGISTER_TABLE_DESCRIPTION) {
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            } else {
                return Qt::AlignCenter;
            }
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool ModbusRegisterTable::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        RegisterItem *newItem = new RegisterItem();
        m_registerItems.insert(row, newItem);
    }
    endInsertRows();

    return true;
}

bool ModbusRegisterTable::removeRows(int row, int count, const QModelIndex &parent)
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