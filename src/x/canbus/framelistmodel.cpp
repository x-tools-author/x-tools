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
    {}

public:
    struct FrameItem
    {
        QString name;
        bool cycle{false};
        int cycleInterval{1000};
        bool response{false};
        int responseId{0};
        QCanBusFrame frame;
    };
    QList<FrameItem> m_items;

private:
    FrameListModel *q{nullptr};
};

FrameListModel::FrameListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    d = new FrameListModelPrivate(this);
}

FrameListModel::~FrameListModel() {}

void FrameListModel::addFrame(const QCanBusFrame &frame, bool isRx) {}

void FrameListModel::clear()
{
    beginResetModel();
    d->m_items.clear();
    endResetModel();
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
            } else if (section == FRAME_LIST_MODEL_COLUMN_RESPONSE_ID) {
                return tr("Response ID");
            } else if (section == FRAME_LIST_MODEL_COLUMN_FRAME) {
                return tr("Frame");
            }
        } else {
            return QString::number(section + 1);
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            if (section == FRAME_LIST_MODEL_COLUMN_FRAME) {
                return int(Qt::AlignVCenter | Qt::AlignLeft);
            }
        }

        return Qt::AlignCenter;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

} // namespace xCanBus
