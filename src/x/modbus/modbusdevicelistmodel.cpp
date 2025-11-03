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
#include "modbusregisterview.h"

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
    return defaultFlags & ~Qt::ItemIsEditable;
}

void ModbusDeviceListModel::newDevice(const QJsonObject &parameters)
{
    DeviceConnectionParameters ctx = json2DeviceConnectionParameters(parameters);
    ModbusDevice *device = new ModbusDevice(parameters, this);
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
        RegisterView *registerView = new RegisterView();
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
    RegisterView *registerView = tableItem->data(USER_ROLE_MODBUS_TABLE).value<RegisterView *>();
    if (!device || !registerView) {
        return;
    }

    for (int i = 0; i < quantity; ++i) {
        ModbusRegisterTable *registerTable = registerView->registerTable();
        RegisterItem item = RegisterItem();
        item.type = type;
        item.address = startAddress + i;
        item.name = QString("0x%1").arg(item.address, 4, 16, QChar('0').toUpper());
        item.unit = "";
        item.min = 0.0;
        item.max = 65535.0;

        RegisterItem *registerItem = registerTable->addRegisterItem(item);
        QStandardItem *stdItem = new QStandardItem(registerItem->name);
        stdItem->setData(QVariant::fromValue(device), USER_ROLE_MODBUS_DEVICE);
        stdItem->setData(QVariant::fromValue(registerView), USER_ROLE_MODBUS_TABLE);
        stdItem->setData(QVariant::fromValue(registerItem), USER_ROLE_MODBUS_REGISTER);
        tableItem->appendRow(stdItem);
    }
}

} // namespace xModbus