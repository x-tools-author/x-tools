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
        ModbusDevice *itemDevice = deviceItem->data(ItemTypeDevice).value<ModbusDevice *>();
        if (itemDevice != device) {
            continue;
        }

        for (int j = 0; j < deviceItem->rowCount(); ++j) {
            QStandardItem *tableItem = deviceItem->child(j);
            for (int k = 0; k < tableItem->rowCount(); ++k) {
                QStandardItem *registerItem = tableItem->child(k);
                ModbusRegister *reg = registerItem->data(ItemTypeRegister).value<ModbusRegister *>();
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
    item->setData(int(ItemTypeDevice), xItemTypeRole);
    item->setData(QVariant::fromValue(device), ItemTypeDevice);
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

    ModbusDevice *device = deviceItem->data(ItemTypeDevice).value<ModbusDevice *>();
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
        registerItem->setData(int(ItemTypeTableView), xItemTypeRole);
        registerItem->setData(QVariant::fromValue(device), ItemTypeDevice);
        registerItem->setData(QVariant::fromValue(registerView), ItemTypeTableView);
        deviceItem->appendRow(registerItem);
        newRegisters(registerItem, group.type, group.startAddress, group.quantity);
    }
}

void ModbusDeviceListModel::newRegisters(QStandardItem *tableItem,
                                         QModbusDataUnit::RegisterType type,
                                         int startAddress,
                                         int quantity)
{
    ModbusDevice *device = tableItem->data(ItemTypeDevice).value<ModbusDevice *>();
    auto *registerView = tableItem->data(ItemTypeTableView).value<ModbusRegisterTableView *>();
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
        stdItem->setData(int(ItemTypeRegister), xItemTypeRole);
        stdItem->setData(QVariant::fromValue(device), ItemTypeDevice);
        stdItem->setData(QVariant::fromValue(registerView), ItemTypeTableView);
        stdItem->setData(QVariant::fromValue(item), ItemTypeRegister);
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
    // Just remove(delete) the data in item.
    // Do not delete the item(QStandardItem) because QStandardItemModel will delete the item.
    QStandardItem *parentItem = this->itemFromIndex(parent);
    if (!parentItem) {
        removeDevices(row, count);
        return QStandardItemModel::removeRows(row, count, parent);
    }

    int parentItemType = parentItem->data(xItemTypeRole).toInt();
    if (parentItemType == ItemTypeDevice) {
        removeTables(row, count, parentItem);
    } else if (parentItemType == ItemTypeTableView) {
        removeRegisters(row, count, parentItem);
    } else {
        qWarning("Can not remove rows from model, unknown parent item type: %d(%s)",
                 parentItemType,
                 parentItem->data(Qt::DisplayRole).toString().toUtf8().constData());
        return false;
    }

    return QStandardItemModel::removeRows(row, count, parent);
}

void ModbusDeviceListModel::removeDevices(int row, int count)
{
    for (int i = row; i < row + count; ++i) {
        QModelIndex deviceIndex = this->index(i, 0, QModelIndex());
        QStandardItem *deviceItem = this->itemFromIndex(deviceIndex);
        ModbusDevice *device = deviceItem->data(ItemTypeDevice).value<ModbusDevice *>();
        if (!device) {
            qWarning("Can not delete device from model, device is nullptr");
            continue;
        }

        int tableCount = deviceItem->rowCount();
        removeTables(0, tableCount, deviceItem);
        device->requestInterruption();
        device->exit();
        device->wait();
        device->deleteLater();
    }
}

void ModbusDeviceListModel::removeTables(int row, int count, QStandardItem *parentItem)
{
    for (int i = row; i < row + count; ++i) {
        if (i >= parentItem->rowCount()) {
            qWarning("Can not delete table from %s, index %d is out of range",
                     parentItem->text().toUtf8().constData(),
                     i);
            continue;
        }

        QStandardItem *viewItem = parentItem->child(i);
        auto *view = viewItem->data(ItemTypeTableView).value<ModbusRegisterTableView *>();
        if (!view) {
            qWarning("Can not delete table view from %s, view is nullptr",
                     parentItem->text().toUtf8().constData());
            continue;
        }

        int registerCount = viewItem->rowCount();
        removeRegisters(0, registerCount, viewItem);
        view->setParent(0);
        view->deleteLater();
    }
}

void ModbusDeviceListModel::removeRegisters(int row, int count, QStandardItem *parentItem)
{
    QModelIndex parentIndex = indexFromItem(parentItem);
    if (parentIndex.isValid() == false) {
        qWarning("Can not delete registers, parent index is invalid");
        return;
    }

    auto *tableView = parentItem->data(ItemTypeTableView).value<ModbusRegisterTableView *>();
    if (!tableView) {
        qWarning("Can not delete registers from %s, tableView is nullptr",
                 tableView->windowTitle().toUtf8().constData());
        return;
    }

    auto *tableModel = tableView->registerTable();
    tableModel->removeRows(row, count, QModelIndex());
}

} // namespace xModbus