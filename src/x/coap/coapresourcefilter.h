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

namespace xCoAP {

class CoAPResourceFilterPrivate;
class CoAPResourceFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    CoAPResourceFilterPrivate *d{nullptr};

public:
    explicit CoAPResourceFilter(QObject *parent = nullptr);
    ~CoAPResourceFilter() override;

    void setFormat(int format);
    void setFilterText(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

} // namespace xCoAP