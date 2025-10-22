/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusdevicelistmodel.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>

#include "modbusdevice.h"

#define USER_ROLE_MODBUS_DEVICE Qt::UserRole + 1

namespace xModbus {

ModbusDeviceListModel::ModbusDeviceListModel(QWidget *parent)
    : QStandardItemModel(parent)
{}

ModbusDeviceListModel::~ModbusDeviceListModel() {}

void ModbusDeviceListModel::addDevice(ModbusDevice *device)
{
    if (!device) {
        return;
    }

    QStandardItem *item = new QStandardItem("Modbus Device");
    item->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    appendRow(item);

    QStandardItem *coilsItem = new QStandardItem(tr("Coils"));
    coilsItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    item->appendRow(coilsItem);

    QStandardItem *discreteInputsItem = new QStandardItem(tr("Discrete Inputs"));
    discreteInputsItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    item->appendRow(discreteInputsItem);

    QStandardItem *holdingRegistersItem = new QStandardItem(tr("Holding Registers"));
    holdingRegistersItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    item->appendRow(holdingRegistersItem);

    QStandardItem *inputRegistersItem = new QStandardItem(tr("Input Registers"));
    inputRegistersItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    item->appendRow(inputRegistersItem);
}

void ModbusDeviceListModel::addRegisterTable(ModbusDevice *device, RegisterModel *model)
{
    Q_UNUSED(device)
    Q_UNUSED(model)
}

Qt::ItemFlags ModbusDeviceListModel::flags(const QModelIndex &index) const
{
    auto defaultFlags = QStandardItemModel::flags(index);
    // Remove Editable flag to make items non-editable
    return defaultFlags & ~Qt::ItemIsEditable;
}

} // namespace xModbus