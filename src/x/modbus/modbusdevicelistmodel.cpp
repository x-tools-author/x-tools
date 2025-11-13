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

#define DEFAULT_TABLE_QUANTITY 10

namespace xModbus {

struct ModbusDeviceListModelKeys
{
    const QString connectionParameters{"connectionParameters"};
    const QString registerTables{"registerTables"};
    const QString registers{"registers"};
};

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
    ModbusDeviceListModelKeys keys;
    QJsonArray array;
    for (int i = 0; i < rowCount(); ++i) {
        QStandardItem *deviceItem = item(i);
        ModbusDevice *device = deviceItem->data(xItemTypeDevice).value<ModbusDevice *>();
        if (!device) {
            qWarning() << "ModbusDeviceListView::save: device is nullptr";
            continue;
        }

        auto ctx = device->parameters();
        QJsonObject ctxObj = deviceConnectionParameters2Json(ctx);
        QJsonObject deviceObj;
        deviceObj.insert(keys.connectionParameters, ctxObj);

        QJsonArray tableObjs;
        for (int j = 0; j < deviceItem->rowCount(); ++j) {
            QStandardItem *tableItem = deviceItem->child(j);
            auto tableView = tableItem->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
            if (!tableView) {
                qWarning() << "ModbusDeviceListView::save: tableView is nullptr";
                continue;
            }

            QJsonObject tableObj = tableView->save();
            tableObjs.append(tableObj);
        }
        deviceObj.insert(keys.registerTables, tableObjs);
        array.append(deviceObj);
    }

    return array;
}

void ModbusDeviceListModel::load(const QJsonArray &array)
{
    ModbusDeviceListModelKeys keys;
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject deviceObj = array.at(i).toObject();
        QJsonObject parametersObj = deviceObj.value(keys.connectionParameters).toObject();
        QStandardItem *deviceItem = newDevice(parametersObj);
        ModbusDevice *device = deviceItem->data(xItemTypeDevice).value<ModbusDevice *>();

        QJsonArray tableObjs = deviceObj.value(keys.registerTables).toArray();
        for (const QJsonValue &tableVal : tableObjs) {
            QJsonObject tableObj = tableVal.toObject();
            QStandardItem *tableItem = newTableView(deviceItem, tableObj);
            auto tableView = tableItem->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
            auto registers = tableView->registerTable()->registerItems();

            for (ModbusRegister *reg : registers) {
                QStandardItem *registerItem = new QStandardItem(reg->name());
                registerItem->setData(int(ItemTypeRegister), xItemTypeRole);
                registerItem->setData(QVariant::fromValue(device), ItemTypeDevice);
                registerItem->setData(QVariant::fromValue(tableView), ItemTypeTableView);
                registerItem->setData(QVariant::fromValue(reg), ItemTypeRegister);
                tableItem->appendRow(registerItem);
            }
        }
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

QStandardItem *ModbusDeviceListModel::newTableView(QStandardItem *deviceItem,
                                                   const QJsonObject &parameters)
{
    ModbusRegisterTableView *tableView = new ModbusRegisterTableView();
    tableView->load(parameters);

    ModbusDevice *device = deviceItem->data(ItemTypeDevice).value<ModbusDevice *>();
    QStandardItem *tableItem = new QStandardItem(tableView->windowTitle());
    tableItem->setData(int(ItemTypeTableView), xItemTypeRole);
    tableItem->setData(QVariant::fromValue(device), ItemTypeDevice);
    tableItem->setData(QVariant::fromValue(tableView), ItemTypeTableView);
    deviceItem->appendRow(tableItem);

    return tableItem;
}

QStandardItem *ModbusDeviceListModel::newRegister(QStandardItem *tableItem,
                                                  const QJsonObject &parameters)
{
    ModbusDevice *device = tableItem->data(ItemTypeDevice).value<ModbusDevice *>();
    auto *tableView = tableItem->data(ItemTypeTableView).value<ModbusRegisterTableView *>();
    auto *table = tableView->registerTable();
    ModbusRegister *reg = new ModbusRegister(tableView->registerTable());
    reg->load(parameters);
    table->addRegisterItem(reg);

    QStandardItem *registerItem = new QStandardItem(reg->name());
    registerItem->setData(int(ItemTypeRegister), xItemTypeRole);
    registerItem->setData(QVariant::fromValue(device), ItemTypeDevice);
    registerItem->setData(QVariant::fromValue(tableView), ItemTypeTableView);
    registerItem->setData(QVariant::fromValue(reg), ItemTypeRegister);
    tableItem->appendRow(registerItem);

    //connect(reg, &ModbusRegister::nameChanged, this, [=]() { registerItem->setText(reg->name); });
    return registerItem;
}

void ModbusDeviceListModel::newDefaultTables(QStandardItem *deviceItem)
{
    QList<QModbusDataUnit::RegisterType> types = {QModbusDataUnit::Coils,
                                                  QModbusDataUnit::DiscreteInputs,
                                                  QModbusDataUnit::HoldingRegisters,
                                                  QModbusDataUnit::InputRegisters};
    for (QModbusDataUnit::RegisterType type : types) {
        ModbusDevice *device = deviceItem->data(ItemTypeDevice).value<ModbusDevice *>();
        ModbusRegisterTableView *tableView = new ModbusRegisterTableView();
        tableView->setWindowTitle(registerTypeToString(type));
        QStandardItem *tableItem = new QStandardItem(registerTypeToString(type));
        tableItem->setData(int(ItemTypeTableView), xItemTypeRole);
        tableItem->setData(QVariant::fromValue(device), ItemTypeDevice);
        tableItem->setData(QVariant::fromValue(tableView), ItemTypeTableView);
        deviceItem->appendRow(tableItem);
        newDefaultRegisters(tableItem, type);
    }
}

void ModbusDeviceListModel::newDefaultRegisters(QStandardItem *tableItem,
                                                QModbusDataUnit::RegisterType type)
{
    QJsonArray registers = defaultTable(type);
    for (int i = 0; i < registers.size(); ++i) {
        QJsonObject registerObj = registers.at(i).toObject();
        newRegister(tableItem, registerObj);
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

QJsonArray ModbusDeviceListModel::defaultTable(QModbusDataUnit::RegisterType type) const
{
    QJsonObject registerObj;
    RegisterItemKeys keys;
    registerObj.insert(keys.type, int(type));
    ModbusRegister item;
    item.load(registerObj);
    QJsonArray array;
    for (int i = 1; i <= DEFAULT_TABLE_QUANTITY; ++i) {
        registerObj.insert(keys.address, i);
        item.load(registerObj);
        array.append(item.save());
    }
    return array;
}

} // namespace xModbus