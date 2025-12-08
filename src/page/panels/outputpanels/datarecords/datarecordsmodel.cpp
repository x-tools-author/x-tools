/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "datarecordsmodel.h"

#include <QDateTime>

#include "common/xtools.h"

DataRecordsModel::DataRecordsModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

DataRecordsModel::~DataRecordsModel() {}

void DataRecordsModel::addRxItem(const QString &flag, const QByteArray &bytes)
{
    addItem(DataRecordsModelItemTypeRx, flag, bytes);
}

void DataRecordsModel::addTxItem(const QString &flag, const QByteArray &bytes)
{
    addItem(DataRecordsModelItemTypeTx, flag, bytes);
}

void DataRecordsModel::addItem(int type, const QString &flag, const QByteArray &bytes)
{
    const int row = m_items.count();

    Item item;
    item.dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    item.type = type;
    item.flag = flag;
    item.bytes = bytes;
    m_items.append(item);

    insertRows(row, 1, QModelIndex());
}

void DataRecordsModel::setFormat(int format)
{
    m_format = format;
    emit dataChanged(index(0, 0), index(m_items.count() - 1, 3));
}

void DataRecordsModel::setMaxRecords(int count)
{
    m_maxRecords = count;
    if (m_maxRecords > 0 && m_items.count() > m_maxRecords) {
        beginResetModel();
        while (m_items.count() > m_maxRecords) {
            m_items.removeFirst();
        }
        endResetModel();
    }
}

void DataRecordsModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

DataRecordsModel::Item DataRecordsModel::itemAt(int row) const
{
    if (row < 0 || row >= m_items.count()) {
        return Item{};
    }

    return m_items.at(row);
}

QString DataRecordsModel::dataText(const Item &item) const
{
    return xBytes2string(item.bytes, m_format);
}

int DataRecordsModel::rowCount(const QModelIndex &parent) const
{
    return m_items.count();
}

int DataRecordsModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

bool DataRecordsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > m_items.count() || count <= 0) {
        return false;
    }

    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

QVariant DataRecordsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= m_items.count()) {
        return QVariant();
    }

    const Item &item = m_items.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case DATARECORDS_COLUMN_COLUMN_DATETIME:
            return item.dateTime;
        case DATARECORDS_COLUMN_COLUMN_TYPE:
            return item.type == DataRecordsModelItemTypeRx ? "Rx" : "Tx";
        case DATARECORDS_COLUMN_COLUMN_FLAG:
            return item.flag;
        case DATARECORDS_COLUMN_COLUMN_DATA:
            return xBytes2string(item.bytes, m_format);
        default:
            return QVariant();
        }
    }

    int column = index.column();
    if (role == Qt::TextAlignmentRole) {
        switch (column) {
        case DATARECORDS_COLUMN_COLUMN_DATETIME:
        case DATARECORDS_COLUMN_COLUMN_TYPE:
        case DATARECORDS_COLUMN_COLUMN_FLAG:
            return Qt::AlignCenter;
        case DATARECORDS_COLUMN_COLUMN_DATA:
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant DataRecordsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case DATARECORDS_COLUMN_COLUMN_DATETIME:
                return tr("Date Time");
            case DATARECORDS_COLUMN_COLUMN_TYPE:
                return tr("Type");
            case DATARECORDS_COLUMN_COLUMN_FLAG:
                return tr("Flag");
            case DATARECORDS_COLUMN_COLUMN_DATA:
                return tr("Data");
            default:
                return QVariant();
            }
        } else if (orientation == Qt::Vertical) {
            return section + 1;
        }
    }

    if (orientation == Qt::Horizontal) {
        if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case DATARECORDS_COLUMN_COLUMN_DATETIME:
            case DATARECORDS_COLUMN_COLUMN_TYPE:
            case DATARECORDS_COLUMN_COLUMN_FLAG:
                return Qt::AlignCenter;
            case DATARECORDS_COLUMN_COLUMN_DATA:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            default:
                return QVariant();
            }
        }
    }

    return QVariant();
}