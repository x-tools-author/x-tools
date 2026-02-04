/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapmsgfilter.h"

#include <QMutex>

#include <coap3/coap.h>

#include "coapmsgmodel.h"

namespace xCoAP {

class CoAPMsgFilterPrivate : public QObject
{
public:
    CoAPMsgFilterPrivate(CoAPMsgFilter* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPMsgFilterPrivate() {}

public:
    QString m_filterText;

private:
    CoAPMsgFilter* q{nullptr};
};

CoAPMsgFilter::CoAPMsgFilter(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    d = new CoAPMsgFilterPrivate(this);
}

CoAPMsgFilter::~CoAPMsgFilter() {}

void CoAPMsgFilter::setFilterText(const QString& text)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    beginFilterChange();
    d->m_filterText = text;
    endFilterChange();
#else
    d->m_filterText = text;
    invalidateFilter();
#endif
}

bool CoAPMsgFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QAbstractItemModel* sourceModel = this->sourceModel();
    CoAPMsgModel* model = qobject_cast<CoAPMsgModel*>(sourceModel);
    if (!model) {
        return false;
    }

    if (sourceRow < 0 || sourceRow >= model->rowCount(sourceParent)) {
        return false;
    }

    QModelIndex index = model->index(sourceRow, CO_AP_MODEL_COLUMN_PAYLOAD, sourceParent);
    QString dataStr = model->data(index, Qt::DisplayRole).toString();

    bool accept = true;
    if (!d->m_filterText.isEmpty()) {
        accept = dataStr.contains(d->m_filterText, Qt::CaseInsensitive);
    }

    return accept;
}

} // namespace xCoAP
