/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSUI_H
#define SAKMODBUSUI_H

#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QTableView>
#include <QJsonArray>
#include <QModbusReply>
#include <QModbusDevice>
#include <QModbusServer>
#include <QModbusDataUnit>
#include <QLoggingCategory>
#include <QStandardItemModel>

namespace Ui {
class SAKModbusUi;
}

class SAKModbusUi : public QWidget {
    Q_OBJECT
public:
    enum SAKEnumModbusDevice {
        ModbusDeviceRtuSerialClient,
        ModbusDeviceRtuSerialServer,
        ModbusDeviceTcpClient,
        ModbusDeviceTcpServer
    };
    Q_ENUM(SAKEnumModbusDevice)

    enum SAKEnumRegisterType {
        DiscreteInputs = QModbusDataUnit::DiscreteInputs,
        Coils = QModbusDataUnit::Coils,
        InputRegisters = QModbusDataUnit::InputRegisters,
        HoldingRegisters = QModbusDataUnit::HoldingRegisters
    };
    Q_ENUM(SAKEnumRegisterType)

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
    Q_ENUM(SAKEnumFunctionCode)

public:
    explicit SAKModbusUi(QWidget *parent = Q_NULLPTR);
    ~SAKModbusUi();

signals:
    void deviceChanged(QModbusDevice *device);

private:
    struct {
        const QString deviceIndex = "SAKModbus/deviceIndex";

        const QString portName = "SAKModbus/portName";
        const QString parity = "SAKModbus/parity";
        const QString baudRate = "SAKModbus/baudRate";
        const QString dataBits = "SAKModbus/dataBits";
        const QString stopBits = "SAKModbus/stopBits";
        const QString customBaudRate = "SAKModbus/customBaudRate";

        const QString address = "SAKModbus/address";
        const QString port = "SAKModbus/port";
        const QString customAddress = "SAKModbus/customAddress";

        const QString clientTimeout = "SAKModbus/clientTimeout";
        const QString clientRepeatTime= "SAKModbus/clientRepeatTime";

        const QString serverIsBusy = "SAKModbus/serverIsBusy";
        const QString serverJustListen= "SAKModbus/serverJustListen";
        const QString serverAddress= "SAKModbus/serverAddress";

        const QString functionCode = "SAKModbus/functionCode";
        const QString targetAddress = "SAKModbus/targetAddress";
        const QString startAddress = "SAKModbus/startAddress";
        const QString addressNumber = "SAKModbus/addressNumber";

        const QString sendHistory = "SAKModbus/sendHistory";
        const QString sendHistoryIndex = "SAKModbus/sendHistoryIndex";
        const QString pdu = "pdu";
    } mSettingsKeyCtx;

private:
    Ui::SAKModbusUi *ui_;
    QModbusDevice *modbus_device_{nullptr};
    QSettings *settings_{nullptr};
    QStandardItemModel *register_model{nullptr};
    const QLoggingCategory logging_category_;

private:
    void InitComponents();
    void InitComponentDevices();
    void InitComponentAddress();
    void InitComponentPortName();
    void InitComponnetBaudRate();
    void InitComponnetDataBits();
    void InitComponnetStopBits();
    void InitComponnetParity();
    void InitComponentFunctionCode();
    void InitComponentRegisterTableView();
    void InitComponentInput();
    void InitComponentRegisterTabWidget();

    void InitSettings();
    void InitSettingsDevice();
    void InitSettingsNetwork();
    void InitSettingsSerialPort();
    void InitSettingsClient();
    void InitSettingsServer();
    void InitSettingsClientOperations();
    void InitSettingsInput();

    void InitSignals();
    void InitSignalsDevice();
    void InitSignalsNetworking();
    void InitSignalsSerialPort();
    void InitSignalsClient();
    void InitSignalsServer();
    void InitSignalsClientOperations();


    void OnErrorOccurred();
    void OnDeviceTypeChanged();
    void OnInvokeClose();
    void OnInvokeOpen();
    void OnAddressChanged();
    void OnPortChanged();
    void OnCustomAddressChanged();
    void OnPortNameChanged();
    void OnParityChanged();
    void OnBaudRateChanged();
    void OnDataBitsChanged();
    void OnStopBistChanged();
    void OnInvokeRefresh();
    void OnClientTimeoutChanged();
    void OnClientRepeatTimeChanged();
    void OnServerIsBusyChanged();
    void OnServerJustListenChanged();
    void OnServerAddressChanged();
    void OnFunctionCodeChanged();
    void OnTargetAddressChanged();
    void OnStartAddressChanged();
    void OnAddressNumberChanged();

private:
    void UpdateUiState(bool opened);
    quint16 CalculateModbusCrc(const QByteArray &data);
    quint16 CookedRegisterString(QString text, int base);
    void SynchronizationRegister(QModbusDevice *server);
    bool WriteSettingsArray(const QString &group, const QString &key,
                            const QString &value, int index, int maxIndex);
    QModbusDataUnit::RegisterType RegisterType(int type);

    // The interfaces of modbus client operations.
    void ClientRead();
    void ClientWrite();
    void ClientSend();
    void ClientUpdateTable();
    void ClientSetRegisterValue(QJsonArray values);
    void ClientUpdateRWBtState();
    void ClientUpdateParameters();
    quint8 ClientFunctionCode();
    QJsonArray ClientRegisterValue();
    QByteArray ClientPdu();

    // Show message box.
    void ShowMessageBox(const QString &title, const QString &msg);
    void ShowMessageBoxDeviceIsNotReady();

    // Output info to ui.
    void OutputModbusReply(QModbusReply *reply, int functionCode);
    void OutputModbusRequestSend(int serverAddress, const QByteArray &pdu);
    void OutputModbusRequestRead(int serverAddress, int functionCode,
                                 int startAddress, int addressNumber);
    void OutputModbusRequestWrite(int serverAddress, int functionCode,
                                  int startAddress, int addressNumber,
                                  QJsonArray values);
    void OutputMessage(const QString &msg, bool isError,
                       const QString &color = QString(),
                       const QString &flag = QString());

    // Table view operations.
    QTableView *TableView(QModbusDataUnit::RegisterType table);
    QTableView *TableViewInit(int rowCount, QTableView *tv);
    QVector<quint16> TableValues(QTableView *tv, int row, int count);
    void TableViewUpdateAddress(QTableView *tv, int startAddress);
    void TableViewUpdateRow(QStandardItem *item);
    void TableViewUpdateData(int table, int address, int size);

    QModbusDevice *CreateRtuSerialDevice(
            int deviceType, const QString &portName, int parity, int baudRate,
            int dataBits, int stopBits);
    QModbusDevice *CreateTcpDevice(
            int deviceType, QString address, int port);
    bool IsClient(QModbusDevice *device);
    bool IsServer(QModbusDevice *device);
    bool ResetServerMap(QModbusDevice *server);
    bool SetClientParameters(
            QModbusDevice *device, int timeout, int numberOfRetries);
    bool SetServerParameters(
            QModbusDevice *device, int option, QVariant value);
    bool OpenDevice(QModbusDevice *device);
    QString ModbuseDeviceErrorString(QModbusDevice *device);
    bool SetServerData(
            QModbusDevice *server, QModbusDataUnit::RegisterType table,
            quint16 address, quint16 data);
    QModbusReply *SendReadRequest(
            QModbusDevice *device, int registerType, int startAddress,
            int size, int serverAddress);
    QJsonArray ServerValues(
            QModbusServer *server, int registerType, int address, int size);
    QModbusReply *SendWriteRequest(
            QModbusDevice *device, int registerType, int startAddress,
            QJsonArray values, int serverAddress);
    bool IsValidModbusReply(QVariant reply);
    QJsonArray ModbusReplyResult(QModbusReply *reply);
    QModbusReply *SendRawRequest(
            QModbusDevice *device, int serverAddress, int functionCode,
            const QByteArray &data);
};

#endif // SAKMODBUSUI_H
