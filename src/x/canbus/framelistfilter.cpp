/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistfilter.h"

#include "framelistcommon.h"
#include "framelistmodel.h"

namespace xCanBus {

class FrameListFilterPrivate : public QObject
{
public:
    FrameListFilterPrivate(FrameListFilter *parent)
        : QObject(parent)
        , q(parent)
    { }

public:
    QString filterString;

private:
    FrameListFilter *q{nullptr};
};

FrameListFilter::FrameListFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    d = new FrameListFilterPrivate(this);
}

FrameListFilter::~FrameListFilter() { }

void FrameListFilter::setFilterString(const QString &filter)
{
    d->filterString = filter;
    setFilterFixedString(filter);
}

bool FrameListFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    auto model = qobject_cast<FrameListModel *>(sourceModel());
    if (!model) {
        return false;
    }

    QList<FrameItem> frameItems = model->frameItems();
    if (source_row < 0 || source_row >= frameItems.size()) {
        return false;
    }

    const FrameItem &item = frameItems.at(source_row);
    QString filterString = d->filterString;
    if (filterString.isEmpty()) {
        return true;
    }

    // Name column
    if (item.name.contains(filterString, Qt::CaseInsensitive)) {
        return true;
    }

    // Payload column
    QString payloadStr = item.frame.payload().toHex(' ').toUpper();
    payloadStr = payloadStr.toUpper();
    if (payloadStr.contains(filterString, Qt::CaseInsensitive)) {
        return true;
    }

    // Frame ID column (check both hex and decimal representations)
    QCanBusFrame::FrameId frameId = item.frame.frameId();
    QString idStr = QString::number(frameId, 16);
    if (idStr.contains(filterString, Qt::CaseInsensitive)) {
        return true;
    }
    idStr = QString::number(frameId, 10);
    if (idStr.contains(filterString, Qt::CaseInsensitive)) {
        return true;
    }

    return false;
}

} // namespace xCanBus