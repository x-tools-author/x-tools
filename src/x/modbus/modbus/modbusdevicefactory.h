/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QObject>

#define gDevMgr ModbusDeviceFactory::singleton()

namespace xModbus {

class ModbusDevice;
class ModbusDeviceFactory : public QObject
{
    Q_OBJECT
private:
    explicit ModbusDeviceFactory(QObject *parent = nullptr);
    ModbusDeviceFactory(const ModbusDeviceFactory &) = delete;
    ModbusDeviceFactory &operator=(const ModbusDeviceFactory &) = delete;

public:
    ~ModbusDeviceFactory() override;

    static ModbusDeviceFactory &singleton();
    ModbusDevice *newDevice(const QJsonObject &parameters, const QString &templateName = QString());

private:
    QList<ModbusDevice *> m_devices;
};

} // namespace xModbus