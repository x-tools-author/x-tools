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

#include "modbuscommon.h"
#include "modbusdevice.h"
#include "modbusregistertable.h"
#include "modbusregistertableview.h"

namespace xModbus {

ModbusDeviceListModel::ModbusDeviceListModel(QWidget *parent)
    : QStandardItemModel(parent)
{}

ModbusDeviceListModel::~ModbusDeviceListModel() {}

Qt::ItemFlags ModbusDeviceListModel::flags(const QModelIndex &index) const
{
    auto defaultFlags = QStandardItemModel::flags(index);
    return defaultFlags & ~Qt::ItemIsEditable;
}

QList<ModbusRegister *> ModbusDeviceListModel::allRegisters(ModbusDevice *device) const
{
    QList<ModbusRegister *> registers;
    for (int i = 0; i < rowCount(); ++i) {
        QStandardItem *deviceItem = this->item(i);
        ModbusDevice *itemDevice = deviceItem->data(USER_ROLE_MODBUS_DEVICE).value<ModbusDevice *>();
        if (itemDevice != device) {
            continue;
        }

        for (int j = 0; j < deviceItem->rowCount(); ++j) {
            QStandardItem *tableItem = deviceItem->child(j);
            for (int k = 0; k < tableItem->rowCount(); ++k) {
                QStandardItem *registerItem = tableItem->child(k);
                ModbusRegister *reg = registerItem->data(USER_ROLE_MODBUS_REGISTER)
                                          .value<ModbusRegister *>();
                if (reg) {
                    registers.append(reg);
                }
            }
        }
    }
    return registers;
}

void ModbusDeviceListModel::newDevice(const QJsonObject &parameters)
{
    DeviceConnectionParameters ctx = json2DeviceConnectionParameters(parameters);
    ModbusDevice *device = new ModbusDevice(this);
    device->setParameters(ctx);
    QStandardItem *item = new QStandardItem(ctx.deviceName);
    item->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    appendRow(item);

    newDefaultTables(item);
}

void ModbusDeviceListModel::newDefaultTables(QStandardItem *deviceItem)
{
    struct RegisterGroup
    {
        QString name;
        QModbusDataUnit::RegisterType type;
        int startAddress;
        int quantity;
    };

    QList<RegisterGroup> registerGroups
        = {{tr("Coils"), QModbusDataUnit::Coils, 0, 10},
           {tr("Discrete Inputs"), QModbusDataUnit::DiscreteInputs, 0, 10},
           {tr("Holding Registers"), QModbusDataUnit::HoldingRegisters, 0, 10},
           {tr("Input Registers"), QModbusDataUnit::InputRegisters, 0, 10}};

    ModbusDevice *device = deviceItem->data(USER_ROLE_MODBUS_DEVICE).value<ModbusDevice *>();
    for (const RegisterGroup &group : registerGroups) {
        ModbusRegisterTableView *registerView = new ModbusRegisterTableView();
        connect(registerView->registerTable(),
                &ModbusRegisterTable::registerValueChanged,
                device,
                &ModbusDevice::setValue);

        registerView->setWindowTitle(group.name);
        QStandardItem *registerItem = new QStandardItem(registerView->windowTitle());
        registerItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
        registerItem->setData(QVariant::fromValue(registerView), USER_ROLE_MODBUS_TABLE);
        deviceItem->appendRow(registerItem);
        newRegisters(registerItem, group.type, group.startAddress, group.quantity);
    }
}

void ModbusDeviceListModel::newRegisters(QStandardItem *tableItem,
                                         QModbusDataUnit::RegisterType type,
                                         int startAddress,
                                         int quantity)
{
    ModbusDevice *device = tableItem->data(USER_ROLE_MODBUS_DEVICE).value<ModbusDevice *>();
    ModbusRegisterTableView *registerView = tableItem->data(USER_ROLE_MODBUS_TABLE)
                                                .value<ModbusRegisterTableView *>();
    if (!device || !registerView) {
        return;
    }

    for (int i = startAddress; i < startAddress + quantity; ++i) {
        ModbusRegisterTable *registerTable = registerView->registerTable();
        ModbusRegister *item = new ModbusRegister(registerTable);
        item->type = type;
        item->address = i;
        item->name = QString("0x%1").arg(item->address, 4, 16, QChar('0').toUpper());
        item->unit = "";
        item->min = 0.0;
        item->max = 65535.0;

        registerTable->addRegisterItem(item);
        QStandardItem *stdItem = new QStandardItem(item->name);
        stdItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
        stdItem->setData(QVariant::fromValue(registerView), USER_ROLE_MODBUS_TABLE);
        stdItem->setData(QVariant::fromValue(item), USER_ROLE_MODBUS_REGISTER);
        tableItem->appendRow(stdItem);
    }
}

} // namespace xModbus