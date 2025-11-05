/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregister.h"

namespace xModbus {

ModbusRegister::ModbusRegister(QObject *parent)
    : QObject(parent)
{
    load(QJsonObject());
}

ModbusRegister::ModbusRegister(const ModbusRegister &other)
{
    type = other.type;
    name = other.name;
    unit = other.unit;
    description = other.description;
    address = other.address;
    min = other.min;
    max = other.max;
    decimals = other.decimals;
    value = other.value;
    serverAddress = other.serverAddress;
}

ModbusRegister::~ModbusRegister() {}

void ModbusRegister::load(const QJsonObject &json)
{
    RegisterItemKeys keys;
    int defaultType = static_cast<int>(QModbusDataUnit::Invalid);
    type = static_cast<QModbusDataUnit::RegisterType>(json.value(keys.type).toInt(defaultType));
    name = json.value(keys.name).toString(QString("Untitled"));
    unit = json.value(keys.unit).toString("");
    description = json.value(keys.description).toString("");
    address = static_cast<quint16>(json.value(keys.address).toInt(1));
    min = json.value(keys.min).toDouble(0);
    max = json.value(keys.max).toDouble(65535);
    decimals = json.value(keys.decimals).toInt(0);
    value = static_cast<qint16>(json.value(keys.value).toInt(0));
    serverAddress = json.value(keys.serverAddress).toInt(1);
}

QJsonObject ModbusRegister::save()
{
    RegisterItemKeys keys;
    QJsonObject json;
    json.insert(keys.type, static_cast<int>(type));
    json.insert(keys.name, name);
    json.insert(keys.unit, unit);
    json.insert(keys.description, description);
    json.insert(keys.address, static_cast<int>(address));
    json.insert(keys.min, min);
    json.insert(keys.max, max);
    json.insert(keys.decimals, decimals);
    json.insert(keys.value, value);
    json.insert(keys.serverAddress, serverAddress);
    return json;
}

quint16 ModbusRegister::getValue() const
{
    return value;
}

void ModbusRegister::setValue(quint16 newValue)
{
    if (value != newValue) {
        value = newValue;
        emit valueChanged();
    }
}

} // namespace xModbus