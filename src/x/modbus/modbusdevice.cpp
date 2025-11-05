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
#include <QTimer>

#include "modbuscommon.h"
#include "modbusregister.h"
#include "modbusregistertable.h"

#define X_MAX_READ_REGISTERS_PER_REQUEST 100

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

    if (isClient()) {
        m_contextMutex.lock();
        updateReadRequestDataUnits();
        m_contextMutex.unlock();
    }
}

void ModbusDevice::setValue(int serverAddress, int registerType, quint16 address, quint16 value)
{
    emit invokeSetValue(serverAddress, registerType, address, value);
}

void ModbusDevice::run()
{
    DeviceConnectionParameters params = parameters();
    m_device = newModbusDevice(params);
    if (!m_device) {
        qInfo() << "Failed to create Modbus device.";
        return;
    }

    // Set connection parameters
    m_device->setConnectionParameter(QModbusDevice::SerialPortNameParameter, params.portName);
    m_device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, params.dataBits);
    m_device->setConnectionParameter(QModbusDevice::SerialParityParameter, params.parity);
    m_device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, params.stopBits);
    m_device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, params.baudRate);
    m_device->setConnectionParameter(QModbusDevice::NetworkAddressParameter, params.tcpAddress);
    m_device->setConnectionParameter(QModbusDevice::NetworkPortParameter, params.tcpPort);

    connect(m_device, &QModbusDevice::stateChanged, m_device, [=](QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            qInfo() << "Modbus device connected.";
            if (isClient()) {
                m_sendReadRequestsTimer->start();
            }
            //emit deviceConnected();
        } else if (state == QModbusDevice::UnconnectedState) {
            qInfo() << "Modbus device disconnected.";
            //emit deviceDisconnected();
        }
    });

    connect(m_device, &QModbusDevice::errorOccurred, m_device, [=]() {
        qInfo() << "Modbus device error occurred:" << m_device->errorString();
        if (isClient() && m_sendReadRequestsTimer) {
            m_sendReadRequestsTimer->stop();
        }
        //emit errorOccurred(device->errorString());
    });

    connect(this,
            &ModbusDevice::invokeSetValue,
            m_device,
            [=](int serverAddress, int registerType, quint16 address, quint16 value) {
                setValueInThreadInner(serverAddress, registerType, address, value);
            });

    if (!m_device->connectDevice()) {
        qInfo() << "Failed to connect Modbus device:" << m_device->errorString();
        return;
    }

    if (isClient()) {
        m_sendReadRequestsTimer = new QTimer();
        m_sendReadRequestsTimer->setInterval(10);
        m_sendReadRequestsTimer->setSingleShot(true);
        connect(m_sendReadRequestsTimer,
                &QTimer::timeout,
                this,
                &ModbusDevice::onSendReadRequestsTimerTimeout,
                Qt::DirectConnection);

        m_readRequestIndex.store(0);
    }

    exec();

    if (m_sendReadRequestsTimer) {
        m_sendReadRequestsTimer->stop();
        m_sendReadRequestsTimer->deleteLater();
        m_sendReadRequestsTimer = nullptr;
    }

    m_device->disconnectDevice();
    m_device->deleteLater();
    m_device = nullptr;
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

void classifyRegisters(const QList<ModbusRegister *> &regs,
                       QMap<int, QMap<int, QList<ModbusRegister *>>> &cache)
{
    for (ModbusRegister *reg : regs) {
        if (reg->type == xCoils) {
            cache[reg->serverAddress][xCoils].append(reg);
        } else if (reg->type == xDiscreteInputs) {
            cache[reg->serverAddress][xDiscreteInputs].append(reg);
        } else if (reg->type == xInputRegisters) {
            cache[reg->serverAddress][xInputRegisters].append(reg);
        } else if (reg->type == xHoldingRegisters) {
            cache[reg->serverAddress][xHoldingRegisters].append(reg);
        }
    }
}

void processRegisters(QList<ModbusRegister *> &regs)
{
    std::sort(regs.begin(), regs.end(), [](ModbusRegister *a, ModbusRegister *b) {
        return a->address < b->address;
    });

    QList<ModbusRegister *> uniqueRegs;
    quint16 lastAddress = static_cast<quint16>(-1);
    for (ModbusRegister *reg : regs) {
        if (reg->address != lastAddress) {
            uniqueRegs.append(reg);
            lastAddress = reg->address;
        }
    }
    regs = uniqueRegs;
}

void createDataUnits(const QList<ModbusRegister *> &regs,
                     QMap<int, QMap<int, QList<QModbusDataUnit>>> &cache)
{
    // cache: QMap<int, QMap<int, QList<QModbusDataUnit>>> key: server address, value: (key: register type, value: data units)
    if (regs.isEmpty()) {
        return;
    }

    int serverAddress = regs.first()->serverAddress;
    QModbusDataUnit::RegisterType registerType = regs.first()->type;
    quint16 startAddress = regs.first()->address;
    quint16 lastAddress = startAddress;
    int count = 1;

    for (int i = 1; i < regs.size(); ++i) {
        ModbusRegister *reg = regs.at(i);
        if (reg->address == lastAddress + 1 && count < X_MAX_READ_REGISTERS_PER_REQUEST) {
            // 地址连续且未超过最大数量，继续合并
            lastAddress = reg->address;
            count++;
        } else {
            // 地址不连续或达到最大数量，创建数据单元
            cache[serverAddress][registerType].append(
                QModbusDataUnit(registerType, startAddress, count));
            // 重置起始地址和计数器
            startAddress = reg->address;
            lastAddress = startAddress;
            count = 1;
        }
    }
    // 创建最后一个数据单元
    cache[serverAddress][registerType].append(QModbusDataUnit(registerType, startAddress, count));
}

void printDataUnits(QMap<int, QMap<int, QList<QModbusDataUnit>>> &cache)
{
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        int serverAddress = it.key();
        qInfo() << "Server Address:" << serverAddress;
        for (auto jt = it.value().begin(); jt != it.value().end(); ++jt) {
            int registerType = jt.key();
            QString typeName;
            switch (registerType) {
            case xCoils:
                typeName = "Coils";
                break;
            case xDiscreteInputs:
                typeName = "Discrete Inputs";
                break;
            case xInputRegisters:
                typeName = "Input Registers";
                break;
            case xHoldingRegisters:
                typeName = "Holding Registers";
                break;
            default:
                typeName = "Unknown";
                break;
            }
            qInfo() << "  Register Type:" << typeName;
            for (const QModbusDataUnit &unit : jt.value()) {
                qInfo() << "    Start Address:" << unit.startAddress()
                        << "Value Count:" << unit.valueCount();
            }
        }
    }
}

void ModbusDevice::updateReadRequestDataUnits()
{
    // 1. 从寄存器列表中按类型分类
    // 2. 移除重复地址，按地址大小排序（从小到大）
    // 3. 创建数据单元，如果地址连续则合并，最多每个数据单元X_MAX_READ_REGISTERS_PER_REQUEST个寄存器
    m_readRequestIndex.store(-1);
    m_pendingReadDataUnits.clear();

    // 1. 从寄存器列表中按类型分类
    // key: server address, value: (key: register type, value: registers)
    QMap<int, QMap<int, QList<ModbusRegister *>>> cache;
    classifyRegisters(m_registers, cache);

    // 2. 移除重复地址，按地址大小排序（从小到大）
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        for (auto jt = it.value().begin(); jt != it.value().end(); ++jt) {
            processRegisters(jt.value());
        }
    }

    // 3. 创建数据单元，如果地址连续则合并，最多每个数据单元X_MAX_READ_REGISTERS_PER_REQUEST个寄存器
    QMap<int, QMap<int, QList<QModbusDataUnit>>> readRequestDataUnits;
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        for (auto jt = it.value().begin(); jt != it.value().end(); ++jt) {
            createDataUnits(jt.value(), readRequestDataUnits);
        }
    }

#if 1
    // Debug: 打印创建的数据单元
    printDataUnits(readRequestDataUnits);
#endif

    for (auto it = readRequestDataUnits.begin(); it != readRequestDataUnits.end(); ++it) {
        int serverAddress = it.key();
        for (auto jt = it.value().begin(); jt != it.value().end(); ++jt) {
            for (const QModbusDataUnit &unit : jt.value()) {
                DataUnit dataUnit;
                dataUnit.serverAddress = serverAddress;
                dataUnit.dataUnit = unit;
                m_pendingReadDataUnits.append(dataUnit);
            }
        }
    }

    if (!m_pendingReadDataUnits.isEmpty()) {
        m_readRequestIndex.store(0);
    }
}

void ModbusDevice::setupModbusReply(QModbusReply *reply)
{
    if (!reply) {
        QModbusClient *client = qobject_cast<QModbusClient *>(m_device);
        qInfo() << "Failed to send read request:" << client->errorString();
        return;
    }

    connect(reply, &QModbusReply::finished, reply, [=]() {
        if (reply->error() == QModbusDevice::NoError) {
            const QModbusDataUnit unit = reply->result();
            qInfo() << "Read response received - Type:" << unit.registerType()
                    << "Start Address:" << unit.startAddress()
                    << "Value Count:" << unit.valueCount();
            // 处理读取的数据
        } else {
            qInfo() << "Read error:" << reply->errorString();
        }

        reply->deleteLater();
    });
}

void ModbusDevice::setValueInThreadInner(int serverAddress,
                                         int registerType,
                                         quint16 address,
                                         quint16 value)
{
    if (!m_device) {
        return;
    }

    if (isClient()) {
        QModbusClient *client = qobject_cast<QModbusClient *>(m_device);
        QModbusDataUnit writeUnit(QModbusDataUnit::RegisterType(registerType),
                                  address,
                                  QList<quint16>() << value);
        QModbusReply *reply = client->sendWriteRequest(writeUnit, serverAddress);
        if (!reply) {
            qInfo() << "Failed to send write request:" << client->errorString();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, [=]() {
            if (reply->error() == QModbusDevice::NoError) {
                qInfo() << "Write response received - Address:" << address << "Value:" << value;
            } else {
                qInfo() << "Write error:" << reply->errorString();
            }

            reply->deleteLater();
        });
    }

    else {
        QModbusServer *server = qobject_cast<QModbusServer *>(m_device);
        bool success = server->setData(QModbusDataUnit::RegisterType(registerType), address, value);
        if (success) {
            qInfo() << "Set value in server - Address:" << address << "Value:" << value;
        } else {
            qInfo() << "Failed to set value in server.";
        }
    }
}

void ModbusDevice::onSendReadRequestsTimerTimeout()
{
    if (m_readRequestIndex.load() == -1) {
        m_sendReadRequestsTimer->setInterval(1000);
        m_sendReadRequestsTimer->start();
        return;
    }

    if (m_readRequestIndex.load() >= m_pendingReadDataUnits.size()) {
        m_sendReadRequestsTimer->setInterval(1000);
        m_readRequestIndex.store(0);
        m_sendReadRequestsTimer->start();
        return;
    }

    DataUnit readUnit = m_pendingReadDataUnits.at(m_readRequestIndex.load());
    QModbusClient *client = qobject_cast<QModbusClient *>(m_device);
    QModbusReply *reply = client->sendReadRequest(readUnit.dataUnit, readUnit.serverAddress);
    setupModbusReply(reply);

    m_readRequestIndex.fetch_add(1);
    m_sendReadRequestsTimer->setInterval(10);
    m_sendReadRequestsTimer->start();
}

void ModbusDevice::onErrorOccurred(QModbusDevice::Error error)
{
    qInfo() << "Modbus device error occurred:" << error;
}

} // namespace xModbus