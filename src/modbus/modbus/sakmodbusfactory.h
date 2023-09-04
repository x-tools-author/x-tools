/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSFACTORY_H
#define SAKMODBUSFACTORY_H

#include <QObject>
#include <QModbusDevice>
#include <QLoggingCategory>

class SAKModbusFactory : public QObject {
    Q_OBJECT
public:
    enum ModbusDeviceType {
        kModbusRtuSerialClient,
        kModbusRtuSerialServer,
        kModbusTcpClient,
        kModbusTcpServer
    };
    Q_ENUM(ModbusDeviceType)

private:
    SAKModbusFactory(QObject *parent = Q_NULLPTR);

public:
    ~SAKModbusFactory();

    static SAKModbusFactory *Instance();
    const QString TypeName(int type);
    QModbusDevice *CreateDevice(int type);
    bool IsTcpDevice(QVariant modbus_device);
    bool IsRtuSerialDevice(QVariant modbus_device);

private:
    const QLoggingCategory kLoggingCategory{"sak.modbus.factory"};
};

#endif // SAKMODBUSFACTORY_H
