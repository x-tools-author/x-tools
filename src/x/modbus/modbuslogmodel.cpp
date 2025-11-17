/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbuslogmodel.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>


namespace xModbus {

QString logTypeToString(LogType type)
{
    switch (type) {
    case LogTypeMsg:
        return QObject::tr("Message");
    case LogTypeWarning:
        return QObject::tr("Warning");
    case LogTypeError:
        return QObject::tr("Error");
    case LogTypeRequest:
        return QObject::tr("Request");
    case LogTypeResponse:
        return QObject::tr("Response");
    default:
        return QString();
    }
}

ModbusLogModel::ModbusLogModel(QObject *parent)
    : QAbstractTableModel(parent)
{
#if 0
    m_logItems.append({QDateTime::currentDateTime(), LogTypeMsg, "Test Message(msg)."});
    m_logItems.append({QDateTime::currentDateTime(), LogTypeWarning, "Test Message(warning)."});
    m_logItems.append({QDateTime::currentDateTime(), LogTypeError, "Test Message(error)."});
    m_logItems.append({QDateTime::currentDateTime(), LogTypeRequest, "Test Message(request)."});
    m_logItems.append({QDateTime::currentDateTime(), LogTypeResponse, "Test Message(response)."});
#endif

    connect(this,
            &ModbusLogModel::invokeAddLog,
            this,
            &ModbusLogModel::onInvokeAddLog,
            Qt::QueuedConnection);
}

ModbusLogModel::~ModbusLogModel() {}

void ModbusLogModel::clearLog()
{
    beginResetModel();
    m_logItems.clear();
    endResetModel();
}

ModbusLogModel &ModbusLogModel::singleton()
{
    static ModbusLogModel instance;
    return instance;
}

void ModbusLogModel::addLogThreadSafely(LogType type, const QString &message)
{
    if (m_ignoreDataLog.load() && (type == LogTypeRequest || type == LogTypeResponse)) {
        return;
    }

    QDateTime currentTime = QDateTime::currentDateTime();
    emit invokeAddLog(currentTime, type, message);
}

QList<ModbusLogModel::LogItem> ModbusLogModel::logItems() const
{
    return m_logItems;
}

void ModbusLogModel::setLogItems(const QList<LogItem> &items)
{
    beginResetModel();
    m_logItems = items;
    endResetModel();
}

void ModbusLogModel::setIgnoreDataLog(bool ignore)
{
    m_ignoreDataLog.store(ignore);
}

void ModbusLogModel::setUsingColor(bool useColor)
{
    beginResetModel();
    m_usingColor = useColor;
    endResetModel();
}

int ModbusLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_logItems.count();
}

int ModbusLogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant ModbusLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_logItems.count())
        return QVariant();

    const LogItem &item = m_logItems.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case MODBUS_LOG_MODEL_COLUMN_DATE_TIME:
            return item.dateTime.toString(Qt::ISODate);
        case MODBUS_LOG_MODEL_COLUMN_TYPE:
            return logTypeToString(item.type);
        case MODBUS_LOG_MODEL_COLUMN_MESSAGE:
            return item.message;
        default:
            return QVariant();
        }
    } else if (role == Qt::EditRole) {
        switch (index.column()) {
        case MODBUS_LOG_MODEL_COLUMN_DATE_TIME:
            return item.dateTime.toString(Qt::ISODate);
        case MODBUS_LOG_MODEL_COLUMN_TYPE:
            return int(item.type);
        case MODBUS_LOG_MODEL_COLUMN_MESSAGE:
            return item.message;
        default:
            return QVariant();
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (index.column() == MODBUS_LOG_MODEL_COLUMN_MESSAGE) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    } else if (role == Qt::BackgroundRole && m_usingColor) {
        if (item.type == LogTypeWarning) {
            return QColor(0xff6b6b);
        } else if (item.type == LogTypeError) {
            return QColor(0xB00020);
        }
    }

    return QVariant();
}

QVariant ModbusLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case MODBUS_LOG_MODEL_COLUMN_DATE_TIME:
            return tr("Time");
        case MODBUS_LOG_MODEL_COLUMN_TYPE:
            return tr("Type");
        case MODBUS_LOG_MODEL_COLUMN_MESSAGE:
            return tr("Message");
        default:
            return QVariant();
        }
    } else if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return QString::number(section + 1);
    } else if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Vertical) {
            return Qt::AlignCenter;
        } else {
            if (section == MODBUS_LOG_MODEL_COLUMN_MESSAGE) {
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            } else {
                return Qt::AlignCenter;
            }
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags ModbusLogModel::flags(const QModelIndex &index) const
{
    auto tmp = QAbstractTableModel::flags(index);
    // Remove ItemIsEditable flag
    tmp &= ~Qt::ItemIsEditable;
    return tmp;
}

void ModbusLogModel::onInvokeAddLog(const QDateTime &time, LogType type, const QString &message)
{
    beginInsertRows(QModelIndex(), m_logItems.count(), m_logItems.count());
    m_logItems.append({time, type, message});
    endInsertRows();
}

} // namespace xModbus
