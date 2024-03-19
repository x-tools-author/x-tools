/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKMODBUSFACTORY_H
#define SAKMODBUSFACTORY_H

#include <QLoggingCategory>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusReply>
#include <QObject>

class SAKModbusFactory : public QObject
{
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

public:
    const QString TypeName(int type);
    QModbusDevice *CreateDevice(int type);
    bool IsTcpDevice(QModbusDevice *modbus_device);
    bool IsRtuSerialDevice(QModbusDevice *modbus_device);
    bool IsTcpDeviceType(int type);
    bool IsRtuSerialDeviceType(int type);
    bool IsServerDevice(QModbusDevice *modbus_device);
    bool IsClientDevice(QModbusDevice *modbus_device);
    bool ConnectDeivce(QModbusDevice *modbus_device);
    bool IsConnected(QModbusDevice *modbus_device);
    bool IsValidModbusReply(QModbusReply *reply);
    bool SetServerData(QModbusDevice *server,
                       QModbusDataUnit::RegisterType table,
                       int address,
                       int data,
                       bool enable_log = true);
    QList<quint16> GetServerData(QModbusDevice *server,
                                 QModbusDataUnit::RegisterType table,
                                 int address,
                                 int quantity);
    void DeleteModbusDevuce(QModbusDevice **modbus_device);

    QModbusDevice *CreateRtuSerialDevice(
        int type, const QString &port_name, int parity, int baud_rate, int data_bits, int stop_bits);
    QModbusDevice *CreateTcpDevice(int deviceType, QString address, int port);
    void SetClientDeviceParameters(QModbusDevice *client, int timeout, int number_of_retries);
    void SetServerDeviceParameters(QModbusDevice *server,
                                   int address,
                                   bool device_busy,
                                   bool listen_only_mode);
    QModbusReply *SendWriteRequest(QModbusDevice *modbus_device,
                                   int register_type,
                                   int start_address,
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
                                   QVector<quint16> values,
#else
                                   QList<quint16> values,
#endif
                                   int server_address);
    QModbusReply *SendRawRequest(QModbusDevice *modbus_device,
                                 int server_address,
                                 int function_code,
                                 const QByteArray &data);

private:
    const QLoggingCategory kLoggingCategory{"SAK.Modbus.Factory"};
};

#endif // SAKMODBUSFACTORY_H
