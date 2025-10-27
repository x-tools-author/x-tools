/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsdictionaryfilter.h"

TsDictionaryFilter::TsDictionaryFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

TsDictionaryFilter::~TsDictionaryFilter() {}

void TsDictionaryFilter::setColumnsVisible(int column, bool visible)
{
    if (visible) {
        m_visibleColumns.insert(column);
    } else {
        m_visibleColumns.remove(column);
    }

    invalidateFilter();
}

bool TsDictionaryFilter::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);
    if (sourceColumn == 0) {
        return true;
    }

    return m_visibleColumns.contains(sourceColumn);
}
