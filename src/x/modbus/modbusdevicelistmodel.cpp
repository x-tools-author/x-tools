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

QJsonArray ModbusDeviceListModel::save() const
{
    QJsonArray array;
    return array;
}

void ModbusDeviceListModel::load(const QJsonArray &array)
{
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject deviceObj = array.at(i).toObject();
        newDevice(deviceObj);
    }
}

QStandardItem *ModbusDeviceListModel::newDevice(const QJsonObject &parameters)
{
    ModbusDevice *device = new ModbusDevice(this);
    DeviceConnectionParameters ctx = json2DeviceConnectionParameters(parameters);
    device->setParameters(ctx);
    QString deviceName = ctx.deviceName;
    if (!device->isClient()) {
        deviceName += QString(" [%1]").arg(ctx.serverAddress);
    }

    QStandardItem *item = new QStandardItem(deviceName);
    item->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
    appendRow(item);
    return item;
}

void ModbusDeviceListModel::newDefaultTables(QStandardItem *deviceItem)
{
    struct RegisterGroup
    {
        QModbusDataUnit::RegisterType type;
        int startAddress;
        int quantity;
    };

    QList<RegisterGroup> registerGroups = {{QModbusDataUnit::Coils, 0, 10},
                                           {QModbusDataUnit::DiscreteInputs, 0, 10},
                                           {QModbusDataUnit::HoldingRegisters, 0, 10},
                                           {QModbusDataUnit::InputRegisters, 0, 10}};

    ModbusDevice *device = deviceItem->data(USER_ROLE_MODBUS_DEVICE).value<ModbusDevice *>();
    for (const RegisterGroup &group : registerGroups) {
        ModbusRegisterTableView *registerView = new ModbusRegisterTableView();
        if (!device->isClient()) {
            registerView->setServerAddressColumnVisible(false);
        }
        connect(registerView->registerTable(),
                &ModbusRegisterTable::registerValueChanged,
                device,
                &ModbusDevice::setValue);

        registerView->setWindowTitle(registerTypeToString(group.type));
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
        QString name = tableItem->text();
        name += QString(" - 0x%1").arg(i, 4, 16, QChar('0').toUpper());
        item->type = type;
        item->address = i;
        item->name = name;
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

Qt::ItemFlags ModbusDeviceListModel::flags(const QModelIndex &index) const
{
    auto defaultFlags = QStandardItemModel::flags(index);
    return defaultFlags & ~Qt::ItemIsEditable;
}

bool ModbusDeviceListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    endRemoveRows();
    return true;
}

} // namespace xModbus