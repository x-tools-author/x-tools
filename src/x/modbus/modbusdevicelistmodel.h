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

#define USER_ROLE_MODBUS_DEVICE Qt::UserRole + 1
#define USER_ROLE_MODBUS_TABLE Qt::UserRole + 2
#define USER_ROLE_MODBUS_REGISTER Qt::UserRole + 3

namespace xModbus {

class ModbusDevice;
class ModbusRegister;
class ModbusDeviceListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ModbusDeviceListModel(QWidget *parent = nullptr);
    ~ModbusDeviceListModel() override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QList<ModbusRegister *> allRegisters(ModbusDevice *device) const;
    QJsonArray save() const;
    void load(const QJsonArray &array);

public:
    QStandardItem *newDevice(const QJsonObject &parameters);
    void newDefaultTables(QStandardItem *deviceItem);
    void newRegisters(QStandardItem *tableItem,
                      QModbusDataUnit::RegisterType type,
                      int startAddress,
                      int quantity);
};

} // namespace xModbus