/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QModbusDataUnit>
#include <QStandardItemModel>

#define xItemTypeRole (Qt::UserRole + 2000)

namespace xModbus {

#define xItemTypeDevice ModbusDeviceListModel::ItemTypeDevice
#define xItemTypeTableView ModbusDeviceListModel::ItemTypeTableView
#define xItemTypeRegister ModbusDeviceListModel::ItemTypeRegister

class ModbusDevice;
class ModbusRegister;
class ModbusDeviceListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum ItemType { ItemTypeDevice = Qt::UserRole + 1000, ItemTypeTableView, ItemTypeRegister };

public:
    explicit ModbusDeviceListModel(QWidget *parent = nullptr);
    ~ModbusDeviceListModel() override;

    QList<ModbusRegister *> allRegisters(ModbusDevice *device) const;
    QJsonArray save() const;
    void load(const QJsonArray &array);

    QStandardItem *newDevice(const QJsonObject &parameters);
    void newDefaultTables(QStandardItem *deviceItem);
    void newRegisters(QStandardItem *tableItem,
                      QModbusDataUnit::RegisterType type,
                      int startAddress,
                      int quantity);

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

private:
    void removeDevices(int row, int count);
    void removeTables(int row, int count, QStandardItem *parentItem);
    void removeRegisters(int row, int count, QStandardItem *parentItem);
};

} // namespace xModbus