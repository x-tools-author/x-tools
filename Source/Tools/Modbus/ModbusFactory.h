/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusReply>
#include <QObject>

class ModbusFactory : public QObject
{
    Q_OBJECT
public:
    enum ModbusDeviceType {
        ModbusRtuSerialClient,
        ModbusRtuSerialServer,
        ModbusTcpClient,
        ModbusTcpServer
    };
    Q_ENUM(ModbusDeviceType)

private:
    ModbusFactory(QObject *parent = Q_NULLPTR);

public:
    ~ModbusFactory();
    static ModbusFactory *Instance();

public:
    const QString typeName(int type);
    QModbusDevice *createDevice(int type);
    bool isTcpDevice(QModbusDevice *modbus_device);
    bool isRtuSerialDevice(QModbusDevice *modbus_device);
    bool isTcpDeviceType(int type);
    bool isRtuSerialDeviceType(int type);
    bool isServerDevice(QModbusDevice *modbus_device);
    bool isClientDevice(QModbusDevice *modbus_device);
    bool connectDeivce(QModbusDevice *modbus_device);
    bool isConnected(QModbusDevice *modbus_device);
    bool isValidModbusReply(QModbusReply *reply);
    bool setServerData(QModbusDevice *server,
                       QModbusDataUnit::RegisterType table,
                       int address,
                       int data,
                       bool enableLog = true);
    QList<quint16> getServerData(QModbusDevice *server,
                                 QModbusDataUnit::RegisterType table,
                                 int address,
                                 int quantity);
    void deleteModbusDevuce(QModbusDevice **modbusDevice);

    QModbusDevice *createRtuSerialDevice(
        int type, const QString &port_name, int parity, int baud_rate, int data_bits, int stop_bits);
    QModbusDevice *createTcpDevice(int deviceType, QString address, int port);
    void setClientDeviceParameters(QModbusDevice *client, int timeout, int number_of_retries);
    void setServerDeviceParameters(QModbusDevice *server, int address, bool isBusy, bool listenOnly);
    QModbusReply *sendWriteRequest(QModbusDevice *modbus_device,
                                   int register_type,
                                   int start_address,
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
                                   QVector<quint16> values,
#else
                                   QList<quint16> values,
#endif
                                   int server_address);
    QModbusReply *sendRawRequest(QModbusDevice *modbus_device,
                                 int server_address,
                                 int function_code,
                                 const QByteArray &data);
};
