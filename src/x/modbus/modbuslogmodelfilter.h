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

namespace xModbus {

class ModbusLogModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ModbusLogModelFilter(QObject *parent = nullptr);
    ~ModbusLogModelFilter() override;

    void setLogType(int type);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    int m_logType{-1};
};

} // namespace xModbus