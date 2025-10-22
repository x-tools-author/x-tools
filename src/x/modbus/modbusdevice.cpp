/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusdevice.h"

#include <QModbusDevice>
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#include <QModbusTcpClient>
#include <QModbusTcpServer>

#include "modbuscommon.h"

#include "modbusregistertable.h"

namespace xModbus {

ModbusDevice::ModbusDevice(QObject *parent)
    : QThread(parent)
{}

ModbusDevice::ModbusDevice(const QJsonObject &parameters, QObject *parent)
    : QThread(parent)
    , m_parameters(parameters)
{}

ModbusDevice::~ModbusDevice()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

QJsonObject ModbusDevice::parameters() const
{
    QJsonObject parameters;
    m_contextMutex.lock();
    parameters = m_parameters;
    m_contextMutex.unlock();
    return parameters;
}

void ModbusDevice::setParameters(const QJsonObject &parameters)
{
    m_contextMutex.lock();
    m_parameters = parameters;
    m_contextMutex.unlock();
}

QJsonObject ModbusDevice::registers() const
{
    QJsonObject registers;
    m_contextMutex.lock();
    registers = m_registers;
    m_contextMutex.unlock();
    return registers;
}

void ModbusDevice::setRegisters(const QJsonObject &registers)
{
    m_contextMutex.lock();
    m_registers = registers;
    m_contextMutex.unlock();
}

bool isModbusClient(int type)
{
    if (type == static_cast<int>(XModbusType::RtuClient)
        || type == static_cast<int>(XModbusType::TcpClient)) {
        return true;
    } else {
        return false;
    }
}

void ModbusDevice::run()
{
    QJsonObject tmp = parameters();
    DeviceConnectionParameters params = json2DeviceConnectionParameters(tmp);
    QModbusDevice *device = nullptr;
    if (isModbusClient(params.deviceType)) {
        if (params.deviceType == static_cast<int>(XModbusType::RtuClient)) {
            device = new QModbusRtuSerialClient();
        } else {
            device = new QModbusTcpClient();
        }
    } else {
        if (params.deviceType == static_cast<int>(XModbusType::RtuServer)) {
            device = new QModbusRtuSerialServer();
        } else {
            device = new QModbusTcpServer();
        }
    }

    connect(device, &QModbusDevice::errorOccurred, device, [=]() {
        qWarning() << "Modbus device error:" << device->errorString();
        emit errorOccurred(device->errorString());
    });
    connect(device, &QModbusDevice::stateChanged, device, [=]() {
        if (device->state() == QModbusDevice::UnconnectedState) {
            emit deviceDisconnected();
        } else if (device->state() == QModbusDevice::ConnectedState) {
            emit deviceConnected();
        }
    });

    // Set connection parameters
    device->setConnectionParameter(QModbusDevice::SerialPortNameParameter, params.portName);
    device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, params.dataBits);
    device->setConnectionParameter(QModbusDevice::SerialParityParameter, params.parity);
    device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, params.stopBits);
    device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, params.baudRate);
    device->setConnectionParameter(QModbusDevice::NetworkAddressParameter, params.tcpAddress);
    device->setConnectionParameter(QModbusDevice::NetworkPortParameter, params.tcpPort);

    // Set additional parameters for client
    QModbusTcpClient *client = qobject_cast<QModbusTcpClient *>(device);
    if (client) {
        client->setNumberOfRetries(params.numberOfRetries);
        client->setTimeout(params.timeout);
        setupClient(client);
    }

    // Set additional parameters for server
    QModbusTcpServer *server = qobject_cast<QModbusTcpServer *>(device);
    if (server) {
        server->setServerAddress(params.serverAddress);
        server->setValue(QModbusServer::ListenOnlyMode, params.listenOnlyMode);
        setupServer(server);
    }

    if (!device->connectDevice()) {
        return;
    }

    exec();
    device->disconnectDevice();
    device->deleteLater();
    device = nullptr;
}

void ModbusDevice::setupClient(QModbusClient *client) {}

void ModbusDevice::setupServer(QModbusServer *server) {}

} // namespace xModbus