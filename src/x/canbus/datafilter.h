/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSortFilterProxyModel>

namespace xCanBus {

class DataFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DataFilter(QObject *parent = nullptr);
    ~DataFilter() override;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

} // namespace xCanBus