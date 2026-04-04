/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "datarecordsmodelfilter.h"

#include "datarecordsmodel.h"

DataRecordsModelFilter::DataRecordsModelFilter(QObject *parent)
    : QSortFilterProxyModel{parent}
{ }

DataRecordsModelFilter::~DataRecordsModelFilter() { }

void DataRecordsModelFilter::setType(int type)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    m_type = type;
    endFilterChange();
#else
    m_type = type;
    invalidateFilter();
#endif
}

void DataRecordsModelFilter::setFlag(const QString &flag)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    m_flag = flag;
    endFilterChange();
#else
    m_flag = flag;
    invalidateFilter();
#endif
}

void DataRecordsModelFilter::setSearchText(const QString &text)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    m_searchText = text;
    endFilterChange();
#else
    m_searchText = text;
    invalidateFilter();
#endif
}

bool DataRecordsModelFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    DataRecordsModel *model = qobject_cast<DataRecordsModel *>(sourceModel());
    if (!model) {
        return false;
    }

    const DataRecordsModel::Item item = model->itemAt(sourceRow);
    if (m_type != DataRecordsModel::DataRecordsModelItemTypeAll && item.type != m_type) {
        return false;
    }

    if (!m_flag.isEmpty() && item.flag != m_flag) {
        return false;
    }

    QString dataText = model->dataText(item);
    if (!m_searchText.isEmpty() && !dataText.contains(m_searchText)) {
        return false;
    }

    return true;
}