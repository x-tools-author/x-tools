/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "datamodel.h"

namespace xCanBus {

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

DataModel::~DataModel() {}

void DataModel::addFrame(const QCanBusFrame &frame, bool isRx)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(FrameItem{isRx, frame});
    endInsertRows();
}

void DataModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

static QString frameFlags(const QCanBusFrame &frame)
{
    QString result{" --- "};

    if (frame.hasBitrateSwitch()) {
        result[1] = u'B';
    }

    if (frame.hasErrorStateIndicator()) {
        result[2] = u'E';
    }

    if (frame.hasLocalEcho()) {
        result[3] = u'L';
    }

    return result;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const int column = index.column();
    const int row = index.row();
    FrameItem item = m_items.at(row);
    QCanBusFrame frame = item.frame;
    if (role == Qt::DisplayRole) {
        if (column == X_CAN_BUS_COLUMN_TIME) {
            return QString::fromLatin1("%1.%2")
                .arg(frame.timeStamp().seconds(), 10, 10, ' ')
                .arg(frame.timeStamp().microSeconds() / 100, 4, 10, '0');
        } else if (column == X_CAN_BUS_COLUMN_TYPE) {
            return item.isRx ? QStringLiteral("Rx") : QStringLiteral("Tx");
        } else if (column == X_CAN_BUS_COLUMN_FLAGS) {
            return frameFlags(frame);
        } else if (column == X_CAN_BUS_COLUMN_ID) {
            return QString::number(frame.frameId(), 16).toUpper();
        } else if (column == X_CAN_BUS_COLUMN_DLC) {
            return QString::number(frame.payload().size());
        } else if (column == X_CAN_BUS_COLUMN_DATA) {
            if (frame.frameType() == QCanBusFrame::ErrorFrame)
                return QString::fromLatin1("<Error Frame>");
            else {
                return QString::fromLatin1(frame.payload().toHex(' ').toUpper());
            }
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == X_CAN_BUS_COLUMN_DATA) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter);
        }
    }

    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case X_CAN_BUS_COLUMN_TIME:
                return tr("Time");
            case X_CAN_BUS_COLUMN_TYPE:
                return tr("Type");
            case X_CAN_BUS_COLUMN_FLAGS:
                return tr("Flags");
            case X_CAN_BUS_COLUMN_ID:
                return tr("ID");
            case X_CAN_BUS_COLUMN_DLC:
                return tr("DLC");
            case X_CAN_BUS_COLUMN_DATA:
                return tr("Payload");
            default:
                qWarning("invalid section %d", section);
                return QVariant();
            }
        } else {
            return QString::number(section + 1);
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            if (section == X_CAN_BUS_COLUMN_DATA) {
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        }

        return Qt::AlignCenter;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

} // namespace xCanBus