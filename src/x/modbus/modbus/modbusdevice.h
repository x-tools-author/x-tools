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

namespace xModbus {

class ModbusDevice : public QThread
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);
    ~ModbusDevice() override;

    QJsonObject parameters() const;
    void setParameters(const QJsonObject &parameters);
    QJsonObject registers() const;
    void setRegisters(const QJsonObject &registers);

signals:
    void deviceConnected();
    void deviceDisconnected();
    void errorOccurred(const QString &errorString);

protected:
    void run() override;

private:
    QJsonObject m_parameters;
    QJsonObject m_registers;
    mutable QMutex m_contextMutex;

private:
    void setupClient(QModbusClient *client);
    void setupServer(QModbusServer *server);
};

} // namespace xModbus