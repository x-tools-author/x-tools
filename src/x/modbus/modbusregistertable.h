/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#include "modbusregister.h"

#define REGISTER_TABLE_ADDRESS 0
#define REGISTER_TABLE_SERVER_ADDRESS 1
#define REGISTER_TABLE_TYPE 2
#define REGISTER_TABLE_MIN 3
#define REGISTER_TABLE_MAX 4
#define REGISTER_TABLE_DECIMALS 5
#define REGISTER_TABLE_NAME 6
#define REGISTER_TABLE_VALUE 7
#define REGISTER_TABLE_UNIT 8
#define REGISTER_TABLE_DESCRIPTION 9

namespace xModbus {

class ModbusRegisterTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModbusRegisterTable(QObject *parent = nullptr);
    ~ModbusRegisterTable() override;

    void addRegisterItem(ModbusRegister *item);
    QList<ModbusRegister *> registerItems() const;

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<ModbusRegister *> m_registerItems;

private:
    void onRegisterItemValueChanged(ModbusRegister *item);
    void onRegisterItemNameChanged(ModbusRegister *item);
};

} // namespace xModbus
