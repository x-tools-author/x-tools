/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "asciimodelfilter.h"

AsciiModelFilter::AsciiModelFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterKeyColumn(4);
}

AsciiModelFilter::~AsciiModelFilter() {}

bool AsciiModelFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    for (int col = 0; col < sourceModel()->columnCount(source_parent); ++col) {
        QModelIndex index = sourceModel()->index(source_row, col, source_parent);
        QString data = sourceModel()->data(index).toString();
        if (data.contains(filterRegularExpression())) {
            return true;
        }
    }

    return false;
}
