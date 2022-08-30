/******************************************************************************
 * Copyright 2022 Qter(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSDEVICEFACTORY_HH
#define SAKMODBUSDEVICEFACTORY_HH

#include <QMutex>
#include <QObject>
#include <QVariant>
#include <QJsonArray>
#include <QModbusReply>
#include <QModbusServer>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QLoggingCategory>

#define SAKMDF SAKModbusDeviceFactory::instance()

class SAKModbusDeviceFactory : public QObject
{
    Q_OBJECT
public:
    enum SAKEnumModbusDevice {
        ModbusDeviceRtuSerialClient,
        ModbusDeviceRtuSerialServer,
        ModbusDeviceTcpClient,
        ModbusDeviceTcpServer
    };
    Q_ENUM(SAKEnumModbusDevice);

    enum SAKEnumRegisterType {
        DiscreteInputs = QModbusDataUnit::DiscreteInputs,
        Coils = QModbusDataUnit::Coils,
        InputRegisters = QModbusDataUnit::InputRegisters,
        HoldingRegisters = QModbusDataUnit::HoldingRegisters
    };
    Q_ENUM(SAKEnumRegisterType);

    enum SAKEnumFunctionCode {
        Invalid = QModbusPdu::Invalid,
        ReadCoils = QModbusPdu::ReadCoils,
        ReadDiscreteInputs = QModbusPdu::ReadDiscreteInputs,
        ReadHoldingRegisters = QModbusPdu::ReadHoldingRegisters,
        ReadInputRegisters = QModbusPdu::ReadInputRegisters,
        WriteSingleCoil = QModbusPdu::WriteSingleCoil,
        WriteSingleRegister = QModbusPdu::WriteSingleRegister,
        ReadExceptionStatus = QModbusPdu::ReadExceptionStatus,
        Diagnostics = QModbusPdu::Diagnostics,
        GetCommEventCounter = QModbusPdu::GetCommEventCounter,
        GetCommEventLog = QModbusPdu::GetCommEventLog,
        WriteMultipleCoils = QModbusPdu::WriteMultipleCoils,
        WriteMultipleRegisters = QModbusPdu::WriteMultipleRegisters,
        ReportServerId = QModbusPdu::ReportServerId,
        ReadFileRecord = QModbusPdu::ReadFileRecord,
        WriteFileRecord = QModbusPdu::WriteFileRecord,
        MaskWriteRegister = QModbusPdu::MaskWriteRegister,
        ReadWriteMultipleRegisters = QModbusPdu::ReadWriteMultipleRegisters,
        ReadFifoQueue = QModbusPdu::ReadFifoQueue,
        EncapsulatedInterfaceTransport =
        QModbusPdu::EncapsulatedInterfaceTransport,
        UndefinedFunctionCode = QModbusPdu::UndefinedFunctionCode
    };
    Q_ENUM(SAKEnumFunctionCode);
private:
    SAKModbusDeviceFactory(QObject *parent = Q_NULLPTR);
public:
    ~SAKModbusDeviceFactory();
    static SAKModbusDeviceFactory *instance();
public:
    Q_INVOKABLE QModbusDevice *createRtuSerialDevice(
        int deviceType, const QString &portName, int parity, int baudRate,
        int dataBits, int stopBits);
    Q_INVOKABLE QModbusDevice *createTcpDevice(
        int deviceType, QString address, int port);
    Q_INVOKABLE bool openDevice(QModbusDevice *device);
    Q_INVOKABLE bool closeDevice(QModbusDevice *device);
    Q_INVOKABLE bool setClientParameters(
        QModbusDevice *device, int timeout, int numberOfRetries);
    Q_INVOKABLE bool setServerParameters(
        QModbusDevice *device, int option, QVariant value);
    Q_INVOKABLE bool isFinishedReply(QVariant reply);
    Q_INVOKABLE QString modbuseDeviceErrorString(QModbusDevice *device);
    Q_INVOKABLE QString modbuseReplyErrorString(QVariant reply);
    Q_INVOKABLE QJsonArray modbusReplyResult(QModbusReply *reply);
    Q_INVOKABLE QByteArray modbusReplyRawData(QModbusReply *reply);
    Q_INVOKABLE QModbusReply *sendReadRequest(
        QModbusDevice *device, int registerType, int startAddress, int size,
        int serverAddress);
    Q_INVOKABLE QModbusReply *sendWriteRequest(
        QModbusDevice *device, int registerType, int startAddress,
        QJsonArray values, int serverAddress);
    Q_INVOKABLE QModbusReply *sendRawRequest(
        QModbusDevice *device, int serverAddress, int functionCode,
        const QByteArray &data);
    Q_INVOKABLE void deleteModbusReply(QVariant modbusReply);
    Q_INVOKABLE bool isClient(QModbusDevice *device);
    Q_INVOKABLE bool isRawResult(QVariant reply);
    Q_INVOKABLE bool isServer(QModbusDevice *device);
    Q_INVOKABLE bool isValidModbusReply(QVariant reply);
    Q_INVOKABLE bool resetServerMap(QVariant server);
    Q_INVOKABLE bool setServerData(
        QModbusDevice *server, QModbusDataUnit::RegisterType table,
        quint16 address, quint16 data);
    Q_INVOKABLE QJsonArray serverValues(
        QModbusServer *server, int registerType, int address, int size);
private:
    static SAKModbusDeviceFactory *m_instance;
    static QMutex m_instanceMutext;
    const QLoggingCategory mLoggingCategory;
};

#endif // SAKMODBUSDEVICEFACTORY_HH
