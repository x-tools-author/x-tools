/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbuslogmodelfilter.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>

#include "modbuslogmodel.h"

namespace xModbus {

ModbusLogModelFilter::ModbusLogModelFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

ModbusLogModelFilter::~ModbusLogModelFilter() {}

void ModbusLogModelFilter::setLogType(int type)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
#else
#endif

    m_logType = type;
    invalidateFilter();
}

bool ModbusLogModelFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex typeIndex = sourceModel()->index(sourceRow,
                                                 MODBUS_LOG_MODEL_COLUMN_TYPE,
                                                 sourceParent);
    if (!typeIndex.isValid()) {
        return false;
    }

    int type = typeIndex.data(Qt::EditRole).toInt();
    bool validType = (m_logType == LogTypeAll) || (type == m_logType);

    for (int i = 0; i < sourceModel()->columnCount(); ++i) {
        QModelIndex idx = sourceModel()->index(sourceRow, i, sourceParent);
        if (!idx.isValid()) {
            return false;
        }

        QString data = sourceModel()->data(idx, Qt::DisplayRole).toString();
        if (data.contains(filterRegularExpression())) {
            return validType;
        }
    }

    return false;
}

} // namespace xModbus