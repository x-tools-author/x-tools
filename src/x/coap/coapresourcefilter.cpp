/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapresourcefilter.h"

#include "coapcommon.h"
#include "coapresourcemodel.h"

namespace xCoAP {

class CoAPResourceFilterPrivate : public QObject
{
public:
    CoAPResourceFilterPrivate(CoAPResourceFilter* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPResourceFilterPrivate() override {}

public:
    int m_format{CO_AP_INVALID_CONTEXT_FORMAT};
    QString m_filterText;

private:
    CoAPResourceFilter* q{nullptr};
};

CoAPResourceFilter::CoAPResourceFilter(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    d = new CoAPResourceFilterPrivate(this);
}

CoAPResourceFilter::~CoAPResourceFilter() {}

void CoAPResourceFilter::setFormat(int format)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    beginFilterChange();
    d->m_format = format;
    endFilterChange();
#else
    d->m_format = format;
    invalidateFilter();
#endif
}

void CoAPResourceFilter::setFilterText(const QString& text)
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

bool CoAPResourceFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QAbstractItemModel* sourceModel = this->sourceModel();
    if (!sourceModel) {
        qCWarning(xCoAPLog) << "Source model is null!";
        return false;
    }

    CoAPResourceModel* model = qobject_cast<CoAPResourceModel*>(sourceModel);
    if (!model) {
        qCWarning(xCoAPLog) << "Source model is not CoAPResourceModel!";
        return false;
    }

    if (sourceRow < 0 || sourceRow >= model->rowCount(sourceParent)) {
        qCWarning(xCoAPLog) << "Invalid source row:" << sourceRow;
        return false;
    }

    QModelIndex formatIndex = model->index(sourceRow, 0, sourceParent);
    QVariant formatVar = model->data(formatIndex, COAP_RES_MODEL_COLUMN_FORMAT);
    if (!formatVar.isValid()) {
        qCWarning(xCoAPLog) << "Invalid format data at row:" << sourceRow;
        return false;
    }

    const auto& payload = formatVar.value<std::shared_ptr<CoAPCommon::PayloadContext>>();
    QModelIndex index = model->index(sourceRow, COAP_RES_MODEL_COLUMN_FORMAT, sourceParent);
    int format = model->data(index, Qt::EditRole).toInt();
    index = model->index(sourceRow, COAP_RES_MODEL_COLUMN_PAYLOAD, sourceParent);
    QString dataStr = model->data(index, Qt::DisplayRole).toString();

    bool accept = true;
    if (d->m_format != CO_AP_INVALID_CONTEXT_FORMAT) {
        accept = (format == d->m_format);
    }
    if (accept && !d->m_filterText.isEmpty()) {
        accept = dataStr.contains(d->m_filterText, Qt::CaseInsensitive);
    }

    return accept;
}

} // namespace xCoAP