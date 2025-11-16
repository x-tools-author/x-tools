/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregister.h"

#include "modbuscommon.h"

namespace xModbus {

ModbusRegister::ModbusRegister(QObject *parent)
    : QObject(parent)
{
    load(QJsonObject());
}

ModbusRegister::ModbusRegister(const ModbusRegister &other)
{
    m_type = other.m_type;
    m_name = other.m_name;
    m_unit = other.m_unit;
    m_description = other.m_description;
    m_address = other.m_address;
    m_min = other.m_min;
    m_max = other.m_max;
    m_decimals = other.m_decimals;
    m_value = other.m_value;
    m_serverAddress = other.m_serverAddress;
}

ModbusRegister::~ModbusRegister() {}

void ModbusRegister::load(const QJsonObject &json)
{
    RegisterItemKeys keys;
    m_address = static_cast<quint16>(json.value(keys.address).toInt(1));
    QString defaultName = QString("0x%1").arg(m_address, 4, 16, QChar('0').toUpper());
    int defaultType = static_cast<int>(QModbusDataUnit::Invalid);
    m_type = static_cast<QModbusDataUnit::RegisterType>(json.value(keys.type).toInt(defaultType));
    m_name = json.value(keys.name).toString(defaultName);
    m_unit = json.value(keys.unit).toString("");
    m_description = json.value(keys.description).toString("");
    m_min = json.value(keys.min).toDouble(0);
    m_max = json.value(keys.max).toDouble(65535);
    m_decimals = json.value(keys.decimals).toInt(0);
    m_value = static_cast<qint16>(json.value(keys.value).toInt(0));
    m_serverAddress = json.value(keys.serverAddress).toInt(1);
}

QJsonObject ModbusRegister::save()
{
    RegisterItemKeys keys;
    QJsonObject json;
    json.insert(keys.type, static_cast<int>(m_type));
    json.insert(keys.name, m_name);
    json.insert(keys.unit, m_unit);
    json.insert(keys.description, m_description);
    json.insert(keys.address, static_cast<int>(m_address));
    json.insert(keys.min, m_min);
    json.insert(keys.max, m_max);
    json.insert(keys.decimals, m_decimals);
    json.insert(keys.value, m_value);
    json.insert(keys.serverAddress, m_serverAddress);
    return json;
}

QModbusDataUnit::RegisterType ModbusRegister::type() const
{
    return m_type;
}

void ModbusRegister::setType(QModbusDataUnit::RegisterType newType)
{
    m_type = newType;
}

QString ModbusRegister::name() const
{
    m_valueMutex.lock();
    QString name = m_name;
    m_valueMutex.unlock();
    return name;
}

void ModbusRegister::setName(const QString &newName)
{
    bool changed = false;
    m_valueMutex.lock();
    if (m_name != newName) {
        m_name = newName;
        changed = true;
    }
    m_valueMutex.unlock();

    if (changed) {
        emit nameChanged();
    }
}

QString ModbusRegister::unit() const
{
    m_valueMutex.lock();
    QString unit = m_unit;
    m_valueMutex.unlock();
    return unit;
}

void ModbusRegister::setUnit(const QString &newUnit)
{
    bool changed = false;
    m_valueMutex.lock();
    if (m_unit != newUnit) {
        m_unit = newUnit;
        changed = true;
    }
    m_valueMutex.unlock();

    if (changed) {
        emit nameChanged();
    }
}

QString ModbusRegister::description() const
{
    m_valueMutex.lock();
    QString description = m_description;
    m_valueMutex.unlock();
    return description;
}

void ModbusRegister::setDescription(const QString &newDescription)
{
    m_valueMutex.lock();
    if (m_description != newDescription) {
        m_description = newDescription;
    }
    m_valueMutex.unlock();
}

quint16 ModbusRegister::address() const
{
    m_valueMutex.lock();
    quint16 address = this->m_address;
    m_valueMutex.unlock();
    return address;
}

void ModbusRegister::setAddress(quint16 newAddress)
{
    m_valueMutex.lock();
    if (m_address != newAddress) {
        m_address = newAddress;
    }
    m_valueMutex.unlock();
}

double ModbusRegister::min() const
{
    m_valueMutex.lock();
    double min = this->m_min;
    m_valueMutex.unlock();
    return min;
}

void ModbusRegister::setMin(double newMin)
{
    m_valueMutex.lock();
    if (m_min != newMin) {
        m_min = newMin;
    }
    m_valueMutex.unlock();
}

double ModbusRegister::max() const
{
    m_valueMutex.lock();
    double max = this->m_max;
    m_valueMutex.unlock();
    return max;
}

void ModbusRegister::setMax(double newMax)
{
    m_valueMutex.lock();
    if (m_max != newMax) {
        m_max = newMax;
    }
    m_valueMutex.unlock();
}

int ModbusRegister::decimals() const
{
    m_valueMutex.lock();
    int decimals = this->m_decimals;
    m_valueMutex.unlock();
    return decimals;
}

void ModbusRegister::setDecimals(int newDecimals)
{
    m_valueMutex.lock();
    if (m_decimals != newDecimals) {
        m_decimals = newDecimals;
    }
    m_valueMutex.unlock();
}

quint16 ModbusRegister::value() const
{
    m_valueMutex.lock();
    quint16 value = this->m_value;
    m_valueMutex.unlock();
    return value;
}

void ModbusRegister::setValue(quint16 newValue)
{
    if (m_type == QModbusDataUnit::Coils || m_type == QModbusDataUnit::DiscreteInputs) {
        newValue = (newValue != 0) ? 1 : 0;
    }

    bool changed = false;
    m_valueMutex.lock();
    if (m_value != newValue) {
        m_value = newValue;
        changed = true;
    }
    m_valueMutex.unlock();

    if (changed) {
        emit valueChanged();
    }
}

int ModbusRegister::serverAddress() const
{
    m_valueMutex.lock();
    int serverAddress = this->m_serverAddress;
    m_valueMutex.unlock();
    return serverAddress;
}

void ModbusRegister::setServerAddress(int newServerAddress)
{
    m_valueMutex.lock();
    if (m_serverAddress != newServerAddress) {
        m_serverAddress = newServerAddress;
    }
    m_valueMutex.unlock();
}

} // namespace xModbus