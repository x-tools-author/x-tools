/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistmodel.h"

#include <QCanBusFrame>
#include <QDebug>

namespace xCanBus {

class FrameListModelPrivate : public QObject
{
public:
    FrameListModelPrivate(FrameListModel *parent)
        : QObject(parent)
        , q(parent)
    { }

public:
    QList<FrameItem> m_items;

private:
    FrameListModel *q{nullptr};
};

FrameListModel::FrameListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    d = new FrameListModelPrivate(this);
}

FrameListModel::~FrameListModel() { }

void FrameListModel::addFrameItem(const FrameItem &frame)
{
    if (!hasFrameId(frame.frame.frameId())) {
        beginInsertRows(QModelIndex(), d->m_items.count(), d->m_items.count());
        d->m_items.append(frame);
        endInsertRows();
        return;
    }

    // If frame with the same ID already exists, update it.
    for (int i = 0; i < d->m_items.count(); ++i) {
        if (d->m_items[i].frame.frameId() == frame.frame.frameId()) {
            d->m_items[i] = frame;
            QModelIndex idx = index(i, 0);
            emit dataChanged(idx, index(i, columnCount(QModelIndex()) - 1));
            break;
        }
    }
}

QList<FrameItem> FrameListModel::frameItems() const
{
    return d->m_items;
}

void FrameListModel::clear()
{
    beginResetModel();
    d->m_items.clear();
    endResetModel();
}

bool FrameListModel::hasFrameId(int frameId) const
{
    for (const FrameItem &item : d->m_items) {
        if (item.frame.frameId() == frameId) {
            return true;
        }
    }
    return false;
}

void FrameListModel::removeFrameItemRow(int row)
{
    if (row < 0 || row >= d->m_items.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    d->m_items.removeAt(row);
    endRemoveRows();
}

int FrameListModel::elapsedTime(int row) const
{
    if (row < 0 || row >= d->m_items.count()) {
        return 0;
    }
    return d->m_items.at(row).elapsed;
}

void FrameListModel::setElapsedTime(int row, int elapsed)
{
    if (row < 0 || row >= d->m_items.count()) {
        return;
    }
    d->m_items[row].elapsed = elapsed;
}

int FrameListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->m_items.count();
}

int FrameListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    if (frame.hasLocalEcho()) {
        result[3] = u'L';
    }
#else
    result[3] = u'-';
#endif

    return result;
}

QVariant FrameListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int column = index.column();
    if (row < 0 || row >= d->m_items.count() || column < 0 || column >= 8) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        if (column == FRAME_LIST_MODEL_COLUMN_PAYLOAD) {
            return int(Qt::AlignVCenter | Qt::AlignLeft);
        }
        return Qt::AlignCenter;
    }

    const FrameItem &item = d->m_items.at(row);
    if (role == Qt::DisplayRole) {
        if (column == FRAME_LIST_MODEL_COLUMN_SEND) {
            return tr("Send");
        } else if (column == FRAME_LIST_MODEL_COLUMN_NAME) {
            return item.name;
        } else if (column == FRAME_LIST_MODEL_COLUMN_CYCLE) {
            return item.response ? tr("Yes") : tr("No");
        } else if (column == FRAME_LIST_MODEL_COLUMN_INTERVAL) {
            return QString::number(item.cycleInterval);
        } else if (column == FRAME_LIST_MODEL_COLUMN_RESPONSE) {
            return item.response ? tr("Yes") : tr("No");
        } else if (column == FRAME_LIST_MODEL_COLUMN_FRAME_ID) {
            return QString::number(item.frame.frameId(), 10).toUpper();
        } else if (column == FRAME_LIST_MODEL_COLUMN_PAYLOAD) {
            return QString::fromLatin1(item.frame.payload().toHex(' ').toUpper());
        }
    }

    return QVariant();
}

QVariant FrameListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == FRAME_LIST_MODEL_COLUMN_SEND) {
                return tr("Options");
            } else if (section == FRAME_LIST_MODEL_COLUMN_NAME) {
                return tr("Name");
            } else if (section == FRAME_LIST_MODEL_COLUMN_CYCLE) {
                return tr("Cycle");
            } else if (section == FRAME_LIST_MODEL_COLUMN_INTERVAL) {
                return tr("Interval(ms)");
            } else if (section == FRAME_LIST_MODEL_COLUMN_RESPONSE) {
                return tr("Response");
            } else if (section == FRAME_LIST_MODEL_COLUMN_FRAME_ID) {
                return tr("Frame ID");
            } else if (section == FRAME_LIST_MODEL_COLUMN_PAYLOAD) {
                return tr("Payload");
            }
        } else {
            return QString::number(section + 1);
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            if (section == FRAME_LIST_MODEL_COLUMN_PAYLOAD) {
                return int(Qt::AlignVCenter | Qt::AlignLeft);
            }
        }

        return Qt::AlignCenter;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

} // namespace xCanBus
