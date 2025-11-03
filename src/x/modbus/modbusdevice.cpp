/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusdevice.h"

#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#include <QModbusTcpClient>
#include <QModbusTcpServer>

#include "modbuscommon.h"
#include "modbusregister.h"
#include "modbusregistertable.h"

namespace xModbus {

ModbusDevice::ModbusDevice(QObject *parent)
    : QThread(parent)
{}

ModbusDevice::~ModbusDevice()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

bool ModbusDevice::isClient() const
{
    m_contextMutex.lock();
    bool result = m_connectionParameters.deviceType == static_cast<int>(XModbusType::RtuClient);
    result |= m_connectionParameters.deviceType == static_cast<int>(XModbusType::TcpClient);
    m_contextMutex.unlock();
    return result;
}

DeviceConnectionParameters ModbusDevice::parameters() const
{
    m_contextMutex.lock();
    DeviceConnectionParameters params = m_connectionParameters;
    m_contextMutex.unlock();
    return params;
}

void ModbusDevice::setParameters(const DeviceConnectionParameters &parameters)
{
    m_contextMutex.lock();
    m_connectionParameters = parameters;
    m_contextMutex.unlock();
}

QList<ModbusRegister *> ModbusDevice::modbusRegisters() const
{
    m_contextMutex.lock();
    QList<ModbusRegister *> regs = m_registers;
    m_contextMutex.unlock();
    return regs;
}

void ModbusDevice::setModbusRegisters(const QList<ModbusRegister *> &registers)
{
    m_contextMutex.lock();
    for (ModbusRegister *reg : registers) {
        m_registers.append(reg);
    }
    m_contextMutex.unlock();
}

void ModbusDevice::run()
{
    DeviceConnectionParameters params = parameters();
    QModbusDevice *device = newModbusDevice(params);
    if (!device) {
        qInfo() << "Failed to create Modbus device.";
        return;
    }

    // Set connection parameters
    device->setConnectionParameter(QModbusDevice::SerialPortNameParameter, params.portName);
    device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, params.dataBits);
    device->setConnectionParameter(QModbusDevice::SerialParityParameter, params.parity);
    device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, params.stopBits);
    device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, params.baudRate);
    device->setConnectionParameter(QModbusDevice::NetworkAddressParameter, params.tcpAddress);
    device->setConnectionParameter(QModbusDevice::NetworkPortParameter, params.tcpPort);

    if (!device->connectDevice()) {
        qInfo() << "Failed to connect Modbus device:" << device->errorString();
        return;
    }

    exec();
    device->disconnectDevice();
    device->deleteLater();
    device = nullptr;
}

QModbusDevice *ModbusDevice::newModbusDevice(const DeviceConnectionParameters &params)
{
    QModbusDevice *device = nullptr;
    if (isClient()) {
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

    // Set additional parameters for client
    QModbusTcpClient *client = qobject_cast<QModbusTcpClient *>(device);
    if (client) {
        client->setNumberOfRetries(params.numberOfRetries);
        client->setTimeout(params.timeout);
    }

    // Set additional parameters for server
    QModbusTcpServer *server = qobject_cast<QModbusTcpServer *>(device);
    if (server) {
        server->setServerAddress(params.serverAddress);
        server->setValue(QModbusServer::ListenOnlyMode, params.listenOnlyMode);
        server->setMap(dataUnitMap());
    }

    connect(device, &QModbusDevice::errorOccurred, device, [=](QModbusDevice::Error error) {
        this->onErrorOccurred(error);
    });

    return device;
}

QModbusDataUnitMap ModbusDevice::dataUnitMap() const
{
    QModbusDataUnitMap dataUnitMap;

    dataUnitMap.insert(xCoils, QModbusDataUnit(xCoils, 0, 65535));
    dataUnitMap.insert(xDiscreteInputs, QModbusDataUnit(xDiscreteInputs, 0, 65535));
    dataUnitMap.insert(xInputRegisters, QModbusDataUnit(xInputRegisters, 0, 65535));
    dataUnitMap.insert(xHoldingRegisters, QModbusDataUnit(xHoldingRegisters, 0, 65535));

    return dataUnitMap;
}

void ModbusDevice::onErrorOccurred(QModbusDevice::Error error)
{
    qInfo() << "Modbus device error occurred:" << error;
}

} // namespace xModbus