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
#include <QModbusClient>
#include <QModbusServer>
#include <QMutex>
#include <QThread>

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

signals:
    void deviceConnected();
    void deviceDisconnected();
    void errorOccurred(const QString &errorString);

protected:
    void run() override;

private:
    DeviceConnectionParameters m_connectionParameters;
    QList<ModbusRegister *> m_registers;
    mutable QMutex m_contextMutex;

private:
    QModbusDevice *newModbusDevice(const DeviceConnectionParameters &params);
    QModbusDataUnitMap dataUnitMap() const;

    void onErrorOccurred(QModbusDevice::Error error);
};

} // namespace xModbus