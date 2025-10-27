/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfilefilter.h"

#include "tsfile.h"
#include "tsitem.h"

TsFileFilter::TsFileFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

TsFileFilter::~TsFileFilter() {}

void TsFileFilter::setFilterType(int type)
{
    if (type < AllItems || type > UnfinishedItems) {
        m_filterType = AllItems;
    } else {
        m_filterType = static_cast<FilterType>(type);
    }

    invalidateFilter();
}

bool TsFileFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_filterType == AllItems) {
        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }

    if (!sourceModel()) {
        return false;
    }

    bool tmp = QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);

    TsFile *tsFile = qobject_cast<TsFile *>(sourceModel());
    TsItem *tsItem = tsFile->tsItemAtLine(sourceRow);
    if (!tsItem) {
        return false;
    }

    if (m_filterType == TranslationItems) {
        return tsItem->isTranslation() && tmp;
    } else if (m_filterType == UnfinishedItems) {
        return tsItem->isTranslation() && (!tsItem->isFinishedTranslation()) && tmp;
    }

    return true;
}