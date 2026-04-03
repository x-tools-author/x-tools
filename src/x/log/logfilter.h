/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSortFilterProxyModel>

namespace xLog {

class LogFilterPrivate;
class LogFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    LogFilterPrivate *d{nullptr};

public:
    explicit LogFilter(QObject *parent = nullptr);
    ~LogFilter() override;

    void setFilterLogLevel(int level);
    void setFilterCategory(const QString &module);
    void setFilterText(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

} // namespace xLog