/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregistertable.h"

#include "modbuscommon.h"

namespace xModbus {

ModbusRegisterTable::ModbusRegisterTable(QObject *parent)
    : QAbstractTableModel(parent)
{}

ModbusRegisterTable::~ModbusRegisterTable() {}

void ModbusRegisterTable::addRegisterItem(ModbusRegister *item)
{
    if (m_registerItems.contains(item)) {
        return;
    }

    beginInsertRows(QModelIndex(), m_registerItems.count(), m_registerItems.count());
    m_registerItems.append(item);
    endInsertRows();
    connect(item, &ModbusRegister::valueChanged, this, [=]() { onRegisterItemValueChanged(item); });
    connect(item, &ModbusRegister::nameChanged, this, [=]() { onRegisterItemNameChanged(item); });
}

QList<ModbusRegister *> ModbusRegisterTable::registerItems() const
{
    return m_registerItems;
}

int ModbusRegisterTable::rowCount(const QModelIndex &parent) const
{
    return m_registerItems.count();
}

int ModbusRegisterTable::columnCount(const QModelIndex &parent) const
{
    return 10;
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

    if (index.row() < 0 || index.row() >= m_registerItems.count()) {
        return QVariant();
    }

    const ModbusRegister *item = m_registerItems.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case REGISTER_TABLE_ADDRESS:
            return QString("0x%1").arg(item->address(), 4, 16, QChar('0').toUpper());
        case REGISTER_TABLE_SERVER_ADDRESS:
            return item->serverAddress();
        case REGISTER_TABLE_NAME:
            return item->name();
        case REGISTER_TABLE_TYPE:
            return registerTypeToString(item->type());
        case REGISTER_TABLE_UNIT:
            return item->unit();
        case REGISTER_TABLE_DESCRIPTION:
            return item->description();
        case REGISTER_TABLE_MIN:
            return item->min();
        case REGISTER_TABLE_MAX:
            return item->max();
        case REGISTER_TABLE_DECIMALS:
            return item->decimals();
        case REGISTER_TABLE_VALUE:
            return QString::number(double(item->value()) / qPow(10, item->decimals()),
                                   'f',
                                   item->decimals());
        default:
            return QVariant();
        }
    } else if (role == Qt::EditRole) {
        switch (index.column()) {
        case REGISTER_TABLE_ADDRESS:
            return item->address();
        case REGISTER_TABLE_SERVER_ADDRESS:
            return item->serverAddress();
        case REGISTER_TABLE_NAME:
            return item->name();
        case REGISTER_TABLE_TYPE:
            return item->type();
        case REGISTER_TABLE_UNIT:
            return item->unit();
        case REGISTER_TABLE_DESCRIPTION:
            return item->description();
        case REGISTER_TABLE_MIN:
            return item->min();
        case REGISTER_TABLE_MAX:
            return item->max();
        case REGISTER_TABLE_DECIMALS:
            return item->decimals();
        case REGISTER_TABLE_VALUE:
            return item->value();
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ModbusRegisterTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    if (index.row() < 0 || index.row() >= m_registerItems.count())
        return false;

    ModbusRegister *item = m_registerItems.at(index.row());
    int column = index.column();
    switch (column) {
    case REGISTER_TABLE_ADDRESS:
        item->setAddress(value.toUInt());
        break;
    case REGISTER_TABLE_SERVER_ADDRESS:
        item->setServerAddress(value.toInt());
        break;
    case REGISTER_TABLE_NAME:
        item->setName(value.toString());
        break;
    case REGISTER_TABLE_TYPE:
        item->setType(static_cast<QModbusDataUnit::RegisterType>(value.toInt()));
        break;
    case REGISTER_TABLE_UNIT:
        item->setUnit(value.toString());
        break;
    case REGISTER_TABLE_DESCRIPTION:
        item->setDescription(value.toString());
        break;
    case REGISTER_TABLE_MIN:
        item->setMin(value.toDouble());
        break;
    case REGISTER_TABLE_MAX:
        item->setMax(value.toDouble());
        break;
    case REGISTER_TABLE_DECIMALS:
        item->setDecimals(value.toInt());
        break;
    case REGISTER_TABLE_VALUE:
        item->setValue(value.toInt());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    if (column == REGISTER_TABLE_DECIMALS) {
        QModelIndex valueIndex = this->index(index.row(), REGISTER_TABLE_VALUE);
        emit dataChanged(valueIndex, valueIndex, QList<int>() << Qt::DisplayRole);
    }

    return true;
}

QVariant ModbusRegisterTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case REGISTER_TABLE_ADDRESS:
                return tr("Register Address");
            case REGISTER_TABLE_NAME:
                return tr("Name");
            case REGISTER_TABLE_SERVER_ADDRESS:
                return tr("Server Address");
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
        ModbusRegister *newItem = new ModbusRegister();
        m_registerItems.insert(row, newItem);
    }
    endInsertRows();

    return true;
}

bool ModbusRegisterTable::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = row; i < row + count; ++i) {
        if (row >= m_registerItems.count()) {
            break;
        }

        ModbusRegister *item = m_registerItems.takeAt(row);
        delete item;
    }
    endRemoveRows();

    return true;
}

Qt::ItemFlags ModbusRegisterTable::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QAbstractTableModel::flags(index);
    }

    auto defaultFlags = QAbstractTableModel::flags(index);
    if (index.column() != REGISTER_TABLE_ADDRESS) {
        return defaultFlags | Qt::ItemIsEditable;
    }

    return defaultFlags;
}

void ModbusRegisterTable::onRegisterItemValueChanged(ModbusRegister *item)
{
    int row = m_registerItems.indexOf(item);
    if (row == -1) {
        return;
    }

    QModelIndex valueIndex = this->index(row, REGISTER_TABLE_VALUE);
    emit dataChanged(valueIndex, valueIndex, QList<int>() << Qt::DisplayRole);
}

void ModbusRegisterTable::onRegisterItemNameChanged(ModbusRegister *item)
{
    int row = m_registerItems.indexOf(item);
    if (row == -1) {
        return;
    }

    QModelIndex nameIndex = this->index(row, REGISTER_TABLE_NAME);
    emit dataChanged(nameIndex, nameIndex, QList<int>() << Qt::DisplayRole);
}

} // namespace xModbus
