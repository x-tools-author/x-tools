/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TableModel.h"

#include <QHostAddress>

TableModel::TableModel(quint32 from, quint32 to, QObject *parent)
    : QAbstractTableModel(parent)
    , m_from(from)
    , m_to(to)
{
    for (quint32 i = from; i <= to; ++i) {
        Ctx ctx{true,
                RawStatusUnknown,
                RawCompareUnknown,
                QHostAddress(i).toString(),
                "---",
                "---",
                "---"};
        m_data.append(ctx);
    }
}
int TableModel::rowCount(const QModelIndex &) const
{
    return m_to - m_from + 1;
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (index.column() == 0) {
            return QString::number(index.row() + 1);
        } else {
            if (index.row() >= m_data.size()) {
                break;
            }

            if (index.column() == 1) {
                return m_data[index.row()].ip;
            } else if (index.column() == 2) {
                return cookedStatus(m_data[index.row()].rawStatus);
            } else if (index.column() == 3) {
                return cookedCompare(m_data[index.row()].rawCompare);
            } else if (index.column() == 4) {
                return m_data[index.row()].description;
            }
        }
    case Qt::EditRole:
        if (index.column() == 0) {
            return m_data[index.row()].isPinging;
        } else if (index.column() == 2) {
            return m_data[index.row()].rawStatus;
        } else if (index.column() == 3) {
            return m_data[index.row()].rawCompare;
        }
        break;
    default:
        break;
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= m_data.size()) {
        return false;
    }

    if (role == Qt::DisplayRole) {
        if (index.column() == 2) {
            m_data[index.row()].status = value.toString();
        } else if (index.column() == 3) {
            m_data[index.row()].compare = value.toString();
        } else if (index.column() == 4) {
            m_data[index.row()].description = value.toString();
        }

        emit dataChanged(index, index);
    } else if (role == Qt::EditRole) {
        if (index.column() == 0) {
            m_data[index.row()].isPinging = value.toBool();
        } else if (index.column() == 2) {
            m_data[index.row()].rawStatus = value.toInt();
        } else if (index.column() == 3) {
            m_data[index.row()].rawCompare = value.toInt();
        }

        emit dataChanged(index, index);
    }

    return true;
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    return {{Qt::DisplayRole, "display"}, {Qt::EditRole, "edit"}};
}

QString TableModel::cookedStatus(int rawStatus)
{
    switch (rawStatus) {
    case RawStatusOnline:
        return tr("Online");
    case RawStatusOffline:
        return tr("Offline");
    default:
        return "---";
    }
}

QString TableModel::cookedCompare(int rawCompare)
{
    switch (rawCompare) {
    case RawCompareEqual:
        return "=";
    case RawCompareAdd:
        return "+";
    case RawCompareSubtract:
        return "-";
    default:
        return "---";
    }
}
