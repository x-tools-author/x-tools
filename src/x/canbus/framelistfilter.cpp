/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistfilter.h"

namespace xCanBus {

FrameListFilter::FrameListFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

FrameListFilter::~FrameListFilter() {}

bool FrameListFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

} // namespace xCanBus