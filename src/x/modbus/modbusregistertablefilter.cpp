/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregistertablefilter.h"

#include "modbusregistertable.h"

namespace xModbus {

ModbusRegisterTableFilter::ModbusRegisterTableFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

ModbusRegisterTableFilter::~ModbusRegisterTableFilter() {}

bool ModbusRegisterTableFilter::filterAcceptsRow(int sourceRow,
                                                 const QModelIndex &sourceParent) const
{
    ModbusRegisterTable *srcModel = qobject_cast<ModbusRegisterTable *>(sourceModel());
    if (!srcModel) {
        return false;
    }

    int columnCount = srcModel->columnCount(sourceParent);
    for (int col = 0; col < columnCount; ++col) {
        QModelIndex index = srcModel->index(sourceRow, col, sourceParent);
        QString txt = srcModel->data(index, Qt::DisplayRole).toString();
        if (txt.contains(filterRegularExpression())) {
            return true;
        }
    }

    return false;
}

} // namespace xModbus