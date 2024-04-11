/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsModbusStudio.h"

#include <QCoreApplication>
#include <QDebug>
#include <QModbusTcpClient>
#include <QModbusTcpServer>
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
#include <QModbusRtuSerialMaster>
#include <QModbusRtuSerialSlave>
#else
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#endif

xToolsModbusStudio::xToolsModbusStudio(QObject *parent)
    : QObject(parent)
{}

xToolsModbusStudio::~xToolsModbusStudio() {}

xToolsModbusStudio *xToolsModbusStudio::Instance()
{
    static xToolsModbusStudio *factory = Q_NULLPTR;

    if (!factory) {
        factory = new xToolsModbusStudio(qApp);
    }

    return factory;
}

const QString xToolsModbusStudio::TypeName(int type)
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

QModbusDevice *xToolsModbusStudio::CreateDevice(int type)
{
    if (type == ModbusRtuSerialClient) {
        qInfo() << "Create rtu serial client.";
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        return new QModbusRtuSerialMaster(this);
#else
        return new QModbusRtuSerialClient(this);
#endif
    } else if (type == ModbusRtuSerialServer) {
        qInfo() << "Create rtu serial server.";
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        return new QModbusRtuSerialSlave(this);
#else
        return new QModbusRtuSerialServer(this);
#endif
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

bool xToolsModbusStudio::IsTcpDevice(QModbusDevice *modbus_device)
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

bool xToolsModbusStudio::IsRtuSerialDevice(QModbusDevice *modbus_device)
{
    if (modbus_device) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
        if (qobject_cast<QModbusRtuSerialClient *>(modbus_device)) {
#else
        if (qobject_cast<QModbusRtuSerialMaster *>(modbus_device)) {
#endif
            return true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
        } else if (qobject_cast<QModbusRtuSerialServer *>(modbus_device)) {
#else
        } else if (qobject_cast<QModbusRtuSerialSlave *>(modbus_device)) {
#endif
            return true;
        }
    }

    return false;
}

bool xToolsModbusStudio::IsTcpDeviceType(int type)
{
    bool is_tcp = (type == ModbusTcpClient);
    is_tcp |= (type == ModbusTcpServer);

    return is_tcp;
}

bool xToolsModbusStudio::IsRtuSerialDeviceType(int type)
{
    bool is_rtu = (type == ModbusRtuSerialClient);
    is_rtu |= (type == ModbusRtuSerialServer);

    return is_rtu;
}

bool xToolsModbusStudio::IsServerDevice(QModbusDevice *modbus_device)
{
    if (modbus_device && qobject_cast<QModbusServer *>(modbus_device)) {
        return true;
    }

    return false;
}

bool xToolsModbusStudio::IsClientDevice(QModbusDevice *modbus_device)
{
    if (modbus_device && qobject_cast<QModbusClient *>(modbus_device)) {
        return true;
    }

    return false;
}

bool xToolsModbusStudio::ConnectDeivce(QModbusDevice *modbus_device)
{
    if (modbus_device) {
        return modbus_device->connectDevice();
    }

    return false;
}

bool xToolsModbusStudio::IsConnected(QModbusDevice *modbus_device)
{
    if (modbus_device) {
        if (modbus_device->state() == QModbusDevice::ConnectedState) {
            return true;
        }
    }

    return false;
}

bool xToolsModbusStudio::IsValidModbusReply(QModbusReply *reply)
{
    if (reply && !reply->isFinished()) {
        return true;
    }

    return false;
}

bool xToolsModbusStudio::SetServerData(QModbusDevice *server,
                                     QModbusDataUnit::RegisterType table,
                                     int address,
                                     int data,
                                     bool enable_log)
{
    bool is_ok = false;
    if (server && qobject_cast<QModbusServer *>(server)) {
        QModbusServer *modbusServer = qobject_cast<QModbusServer *>(server);
        is_ok = modbusServer->setData(table, address, data);
        if (enable_log) {
            qInfo() << "Set register data result:" << is_ok << "table:" << table
                    << "address:" << address << "data:" << data;
        }
    }

    return is_ok;
}

QList<quint16> xToolsModbusStudio::GetServerData(QModbusDevice *server,
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

void xToolsModbusStudio::DeleteModbusDevuce(QModbusDevice **modbus_device)
{
    if (*modbus_device) {
        QModbusServer *server = qobject_cast<QModbusServer *>(*modbus_device);
        if (server) {
            server->disconnect();
        }

        (*modbus_device)->deleteLater();
        (*modbus_device) = Q_NULLPTR;
    }
}

QModbusDevice *xToolsModbusStudio::CreateRtuSerialDevice(
    int type, const QString &port_name, int parity, int baud_rate, int data_bits, int stop_bits)
{
    QModbusDevice *device = CreateDevice(type);
    if (IsRtuSerialDevice(device)) {
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

QModbusDevice *xToolsModbusStudio::CreateTcpDevice(int type, QString address, int port)
{
    QModbusDevice *device = CreateDevice(type);
    if (IsTcpDevice(device)) {
        device->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address);
        device->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);

        qInfo() << "Set tcp modbus device parameters:"
                << "ip address:" << address << "port" << port;
    }

    return device;
}

void xToolsModbusStudio::SetClientDeviceParameters(QModbusDevice *client,
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

void xToolsModbusStudio::SetServerDeviceParameters(QModbusDevice *server,
                                                 int address,
                                                 bool device_busy,
                                                 bool listen_only_mode)
{
    if (server) {
        qInfo() << "Set modbus server device parameters:"
                << "address:" << address << "device busy" << device_busy
                << "listen only mode:" << listen_only_mode;

        QModbusServer *cooked_server = qobject_cast<QModbusServer *>(server);
        cooked_server->setServerAddress(address);
        cooked_server->setValue(QModbusServer::DeviceBusy, device_busy);
        cooked_server->setValue(QModbusServer::ListenOnlyMode, listen_only_mode);
    }
}

QModbusReply *xToolsModbusStudio::SendWriteRequest(QModbusDevice *modbus_device,
                                                 int register_type,
                                                 int start_address,
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
                                                 QVector<quint16> values,
#else
                                                 QList<quint16> values,
#endif
                                                 int server_address)
{
    if (modbus_device && IsClientDevice(modbus_device)) {
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

QModbusReply *xToolsModbusStudio::SendRawRequest(QModbusDevice *modbus_device,
                                               int server_address,
                                               int function_code,
                                               const QByteArray &data)
{
    auto cooked_function_code = QModbusPdu::FunctionCode(function_code);
    QModbusRequest request(cooked_function_code, data);
    if (IsClientDevice(modbus_device)) {
        QModbusClient *client = qobject_cast<QModbusClient *>(modbus_device);
        return client->sendRawRequest(request, server_address);
    }

    return Q_NULLPTR;
}
