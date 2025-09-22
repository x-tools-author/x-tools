/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusdevicefactory.h"

#include "modbusdevice.h"

namespace xModbus {

ModbusDeviceFactory::ModbusDeviceFactory(QObject *parent)
    : QObject(parent)
{}

ModbusDeviceFactory::~ModbusDeviceFactory() {}

ModbusDeviceFactory &ModbusDeviceFactory::singleton()
{
    static ModbusDeviceFactory singleton;
    return singleton;
}

ModbusDevice *ModbusDeviceFactory::newDevice(const QJsonObject &parameters,
                                             const QString &templateName)
{
    ModbusDevice *device = new ModbusDevice(this);
    m_devices.append(device);
    return device;
}

} // namespace xModbus