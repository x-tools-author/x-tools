/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusdevicelistmodelfilter.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>

namespace xModbus {

ModbusDeviceListModelFilter::ModbusDeviceListModelFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

ModbusDeviceListModelFilter::~ModbusDeviceListModelFilter() {}

bool ModbusDeviceListModelFilter::filterAcceptsRow(int sourceRow,
                                                   const QModelIndex &sourceParent) const
{
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

} // namespace xModbus