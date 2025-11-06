/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <QJsonObject>
#include <QMap>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusServer>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include "modbuscommon.h"
#include "modbusregister.h"

namespace xModbus {

class ModbusRegister;
struct DeviceConnectionParameters;
class ModbusDevice : public QThread
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);
    ~ModbusDevice() override;

    bool isClient() const;

    DeviceConnectionParameters parameters() const;
    void setParameters(const DeviceConnectionParameters &parameters);
    QList<ModbusRegister *> modbusRegisters() const;
    void setModbusRegisters(const QList<ModbusRegister *> &registers);
    void setValue(int serverAddress, int registerType, quint16 address, quint16 value);

protected:
    void run() override;

private:
    Q_SIGNAL void invokeSetValue(int serverAddress,
                                 int registerType,
                                 quint16 address,
                                 quint16 value);

private:
    DeviceConnectionParameters m_connectionParameters;
    QList<ModbusRegister *> m_registers;
    mutable QMutex m_contextMutex;

    QModbusDevice *m_device{nullptr};
    std::atomic_int m_readRequestIndex{0};
    QTimer *m_sendReadRequestsTimer = nullptr;

    struct DataUnit
    {
        int serverAddress;
        QModbusDataUnit dataUnit;
    };
    QList<DataUnit> m_pendingReadDataUnits;

private:
    QModbusDevice *newModbusDevice(const DeviceConnectionParameters &params);
    QModbusDataUnitMap dataUnitMap() const;
    void updateReadRequestDataUnits();
    void setupModbusReply(QModbusReply *reply);
    void setValueInThreadInner(int serverAddress, int registerType, quint16 address, quint16 value);

    void onSendReadRequestsTimerTimeout();
    void onDataWritten(QModbusDataUnit::RegisterType table, int address, int size);
};

} // namespace xModbus