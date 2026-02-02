/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadfilter.h"

#include <QMutex>

#include <coap3/coap.h>

#include "coapcommon.h"
#include "coappayloadmodel.h"

namespace xCoAP {

class CoAPPayloadFilterPrivate : public QObject
{
public:
    CoAPPayloadFilterPrivate(CoAPPayloadFilter* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPPayloadFilterPrivate() override {}

public:
    struct CoAPMsgItem
    {
        bool isRx;
        const QString host;
        const quint16 port;
        const coap_pdu_t* pdu;
    };
    QList<CoAPMsgItem> m_msgList;
    QMutex m_msgListMutex;
    int m_format{-1};
    QString m_filterText;

private:
    CoAPPayloadFilter* q{nullptr};
};

CoAPPayloadFilter::CoAPPayloadFilter(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    d = new CoAPPayloadFilterPrivate(this);
}

CoAPPayloadFilter::~CoAPPayloadFilter() {}

void CoAPPayloadFilter::setFormat(int format)
{
    d->m_format = format;
    invalidateFilter();
}

void CoAPPayloadFilter::setFilterText(const QString& text)
{
    d->m_filterText = text;
    invalidateFilter();
}

bool CoAPPayloadFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QAbstractItemModel* sourceModel = this->sourceModel();
    if (!sourceModel) {
        return false;
    }

    CoAPPayloadModel* model = qobject_cast<CoAPPayloadModel*>(sourceModel);
    if (!model) {
        return false;
    }

    if (sourceRow < 0 || sourceRow >= model->rowCount(sourceParent)) {
        return false;
    }

    QModelIndex formatIndex = model->index(sourceRow, 0, sourceParent);
    QVariant formatVar = model->data(formatIndex, CO_AP_PAYLOAD_DATA_ROLE_PAYLOAD);
    if (!formatVar.isValid()) {
        return false;
    }

    const auto& payload = formatVar.value<std::shared_ptr<CoAPCommon::PayloadContext>>();
    QModelIndex index = model->index(sourceRow, CO_AP_PAYLOAD_MODEL_COLUMN_FORMAT, sourceParent);
    int format = model->data(index, Qt::EditRole).toInt();
    index = model->index(sourceRow, CO_AP_PAYLOAD_MODEL_COLUMN_DATA, sourceParent);
    QString dataStr = model->data(index, Qt::DisplayRole).toString();

    bool accept = true;
    if (d->m_format != -1) {
        accept = (format == d->m_format);
    }
    if (accept && !d->m_filterText.isEmpty()) {
        accept = dataStr.contains(d->m_filterText, Qt::CaseInsensitive);
    }

    return accept;
}

} // namespace xCoAP
