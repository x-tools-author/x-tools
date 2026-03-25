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
{}

DataRecordsModelFilter::~DataRecordsModelFilter() {}

void DataRecordsModelFilter::setType(int type)
{
    m_type = type;
    invalidateFilter();
}

void DataRecordsModelFilter::setFlag(const QString &flag)
{
    m_flag = flag;
    invalidateFilter();
}

void DataRecordsModelFilter::setSearchText(const QString &text)
{
    m_searchText = text;
    invalidateFilter();
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