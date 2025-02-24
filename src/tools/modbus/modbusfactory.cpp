/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusfactory.h"

#include <QCoreApplication>
#include <QDebug>
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#include <QModbusTcpClient>
#include <QModbusTcpServer>

ModbusFactory::ModbusFactory(QObject *parent)
    : QObject(parent)
{}

ModbusFactory::~ModbusFactory() {}

ModbusFactory *ModbusFactory::Instance()
{
    static ModbusFactory *factory = Q_NULLPTR;

    if (!factory) {
        factory = new ModbusFactory(qApp);
    }

    return factory;
}

const QString ModbusFactory::typeName(int type)
{
    if (type == ModbusRtuSerialClient) {
        return tr("RTU Client");
    } else if (type == ModbusRtuSerialServer) {
        return tr("RTU Server");
    } else if (type == ModbusTcpClient) {
        return tr("TCP Client");
    } else if (type == ModbusTcpServer) {
        return tr("TCP Server");
    }

    Q_ASSERT_X(false, __FUNCTION__, "Unknown modebus device type");
    qWarning() << "Unknown modebus device type";

    return "Unknown";
}

QModbusDevice *ModbusFactory::createDevice(int type)
{
    if (type == ModbusRtuSerialClient) {
        qInfo() << "Create rtu serial client.";
        return new QModbusRtuSerialClient(this);
    } else if (type == ModbusRtuSerialServer) {
        qInfo() << "Create rtu serial server.";
        return new QModbusRtuSerialServer(this);
    } else if (type == ModbusTcpClient) {
        qInfo() << "Create tcp client.";
        return new QModbusTcpClient();
    } else if (type == ModbusTcpServer) {
        qInfo() << "Create tcp server.";
        return new QModbusTcpServer();
    }

    Q_ASSERT_X(false, __FUNCTION__, "Unknown modebus device type");
    qWarning() << "Unknown modebus device type";

    return Q_NULLPTR;
}

bool ModbusFactory::isTcpDevice(QModbusDevice *modbus_device)
{
    if (modbus_device) {
        if (qobject_cast<QModbusTcpClient *>(modbus_device)) {
            return true;
        } else if (qobject_cast<QModbusTcpServer *>(modbus_device)) {
            return true;
        }
    }

    return false;
}

bool ModbusFactory::isRtuSerialDevice(QModbusDevice *modbus_device)
{
    if (modbus_device) {
        if (qobject_cast<QModbusRtuSerialClient *>(modbus_device)) {
            return true;
        } else if (qobject_cast<QModbusRtuSerialServer *>(modbus_device)) {
            return true;
        }
    }

    return false;
}

bool ModbusFactory::isTcpDeviceType(int type)
{
    bool is_tcp = (type == ModbusTcpClient);
    is_tcp |= (type == ModbusTcpServer);

    return is_tcp;
}

bool ModbusFactory::isRtuSerialDeviceType(int type)
{
    bool is_rtu = (type == ModbusRtuSerialClient);
    is_rtu |= (type == ModbusRtuSerialServer);

    return is_rtu;
}

bool ModbusFactory::isServerDevice(QModbusDevice *modbus_device)
{
    if (modbus_device && qobject_cast<QModbusServer *>(modbus_device)) {
        return true;
    }

    return false;
}

bool ModbusFactory::isClientDevice(QModbusDevice *modbus_device)
{
    if (modbus_device && qobject_cast<QModbusClient *>(modbus_device)) {
        return true;
    }

    return false;
}

bool ModbusFactory::connectDeivce(QModbusDevice *modbus_device)
{
    if (modbus_device) {
        return modbus_device->connectDevice();
    }

    return false;
}

bool ModbusFactory::isConnected(QModbusDevice *modbus_device)
{
    if (modbus_device) {
        if (modbus_device->state() == QModbusDevice::ConnectedState) {
            return true;
        }
    }

    return false;
}

bool ModbusFactory::isValidModbusReply(QModbusReply *reply)
{
    if (reply && !reply->isFinished()) {
        return true;
    }

    return false;
}

bool ModbusFactory::setServerData(QModbusDevice *server,
                                  QModbusDataUnit::RegisterType table,
                                  int address,
                                  int data,
                                  bool enableLog)
{
    bool is_ok = false;
    if (server && qobject_cast<QModbusServer *>(server)) {
        QModbusServer *modbusServer = qobject_cast<QModbusServer *>(server);
        is_ok = modbusServer->setData(table, address, data);
        if (enableLog) {
#if 0
            qInfo() << "Set register data result:" << is_ok << "table:" << table
                    << "address:" << address << "data:" << data;
#endif
        }
    }

    return is_ok;
}

QList<quint16> ModbusFactory::getServerData(QModbusDevice *server,
                                            QModbusDataUnit::RegisterType table,
                                            int address,
                                            int quantity)
{
    QList<quint16> values;
    if (server && qobject_cast<QModbusServer *>(server)) {
        QModbusServer *cooked_server = qobject_cast<QModbusServer *>(server);
        for (int i = address; i < quantity; i++) {
            quint16 value;
            if (cooked_server->data(table, i, &value)) {
                values.append(value);
            } else {
                qWarning() << "Parameters error!";
                break;
            }
        }
    } else {
        qWarning() << "Can not get values from null object!";
    }

    return values;
}

void ModbusFactory::deleteModbusDevuce(QModbusDevice **modbusDevice)
{
    if (*modbusDevice) {
        QModbusServer *server = qobject_cast<QModbusServer *>(*modbusDevice);
        if (server) {
            server->disconnect();
        }

        (*modbusDevice)->deleteLater();
        (*modbusDevice) = Q_NULLPTR;
    }
}

QModbusDevice *ModbusFactory::createRtuSerialDevice(
    int type, const QString &port_name, int parity, int baud_rate, int data_bits, int stop_bits)
{
    QModbusDevice *device = createDevice(type);
    if (isRtuSerialDevice(device)) {
        device->setConnectionParameter(QModbusDevice::SerialPortNameParameter, port_name);
        device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baud_rate);
        device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, data_bits);
        device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stop_bits);
        device->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);

        qInfo() << "Set rtu serial modbus device parameters:"
                << "port name:" << port_name << "baud rate:" << baud_rate
                << "data bits:" << data_bits << "stop bits" << stop_bits << "parity" << parity;
    }

    return device;
}

QModbusDevice *ModbusFactory::createTcpDevice(int type, QString address, int port)
{
    QModbusDevice *device = createDevice(type);
    if (isTcpDevice(device)) {
        device->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address);
        device->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);

        qInfo() << "Set tcp modbus device parameters:"
                << "ip address:" << address << "port" << port;
    }

    return device;
}

void ModbusFactory::setClientDeviceParameters(QModbusDevice *client,
                                              int timeout,
                                              int number_of_retries)
{
    if (client) {
        qInfo() << "Set modbus client device parameters:"
                << "timeout:" << timeout << "number_of_retries" << number_of_retries;

        QModbusClient *cooked_client = qobject_cast<QModbusClient *>(client);
        cooked_client->setTimeout(timeout);
        cooked_client->setNumberOfRetries(number_of_retries);
    }
}

void ModbusFactory::setServerDeviceParameters(QModbusDevice *server,
                                              int address,
                                              bool isBusy,
                                              bool listenOnly)
{
    if (server) {
        qInfo() << "Set modbus server device parameters:"
                << "address:" << address << "device busy" << isBusy
                << "listen only mode:" << listenOnly;

        QModbusServer *cooked_server = qobject_cast<QModbusServer *>(server);
        cooked_server->setServerAddress(address);
        cooked_server->setValue(QModbusServer::DeviceBusy, isBusy);
        cooked_server->setValue(QModbusServer::ListenOnlyMode, listenOnly);
    }
}

QModbusReply *ModbusFactory::sendWriteRequest(QModbusDevice *modbus_device,
                                              int register_type,
                                              int start_address,
                                              QList<quint16> values,
                                              int server_address)
{
    if (modbus_device && isClientDevice(modbus_device)) {
        auto cooked_type = QModbusDataUnit::RegisterType(register_type);
        QModbusDataUnit dataUnit(cooked_type, start_address, values);
        if (dataUnit.isValid()) {
            qInfo() << "register-type:" << register_type << " start-address:" << start_address
                    << " serverAddress:" << server_address << " values:" << values;

            auto *client = qobject_cast<QModbusClient *>(modbus_device);
            QModbusReply *reply = client->sendWriteRequest(dataUnit, server_address);
            return reply;
        } else {
            qWarning() << "Unvalid data unit!";
        }
    }

    return Q_NULLPTR;
}

QModbusReply *ModbusFactory::sendRawRequest(QModbusDevice *modbus_device,
                                            int server_address,
                                            int function_code,
                                            const QByteArray &data)
{
    auto cooked_function_code = QModbusPdu::FunctionCode(function_code);
    QModbusRequest request(cooked_function_code, data);
    if (isClientDevice(modbus_device)) {
        QModbusClient *client = qobject_cast<QModbusClient *>(modbus_device);
        return client->sendRawRequest(request, server_address);
    }

    return Q_NULLPTR;
}
