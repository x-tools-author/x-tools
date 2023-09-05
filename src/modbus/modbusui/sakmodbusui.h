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
        const QString device_index = "SAKModbus/deviceIndex";

        const QString port_name = "SAKModbus/portName";
        const QString parity = "SAKModbus/parity";
        const QString baud_rate = "SAKModbus/baudRate";
        const QString data_bits = "SAKModbus/dataBits";
        const QString stop_bits = "SAKModbus/stopBits";
        const QString custom_baud_rate = "SAKModbus/customBaudRate";

        const QString address = "SAKModbus/address";
        const QString port = "SAKModbus/port";
        const QString custom_address = "SAKModbus/customAddress";

        const QString client_timeout = "SAKModbus/clientTimeout";
        const QString client_repeat_time= "SAKModbus/clientRepeatTime";

        const QString server_is_busy = "SAKModbus/serverIsBusy";
        const QString server_just_listen= "SAKModbus/serverJustListen";
        const QString server_address= "SAKModbus/serverAddress";

        const QString function_code = "SAKModbus/functionCode";
        const QString target_address = "SAKModbus/targetAddress";
        const QString start_address = "SAKModbus/startAddress";
        const QString address_number = "SAKModbus/addressNumber";

        const QString send_history = "SAKModbus/sendHistory";
        const QString send_history_index = "SAKModbus/sendHistoryIndex";
        const QString pdu = "pdu";
    } key_ctx_;

  private:
    Ui::SAKModbusUi *ui_;
    QModbusDevice *modbus_device_{Q_NULLPTR};
    QSettings *settings_{Q_NULLPTR};
    QStandardItemModel *register_model_{Q_NULLPTR};
    const QLoggingCategory kLoggingCategory;

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
    void OnCloseClicked();
    void OnOpenClicked();
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


    void OnReadClicked();
    void OnWriteClicked();
    void OnSendClicked();

    void OnDateWritten(QModbusDataUnit::RegisterType table,
                       int address, int size);
    void OnItemChanged(QStandardItem *item);

  private:
    QModbusDevice *CreateModbusDevice();
    QTableView *CreateTableView(int row_count, QTableView *table_view);

    void UpdateUiState(bool connected);
    void UpdateClientTableView();
    void UpdateClientTableViewData(const QList<quint16> &values);
    void UpdateClientReadWriteButtonState();
    void UpdateClientParameters();
    void UpdateClientTableViewAddress(QTableView *view, int start_address);
    void UpdateServerParameters();
    bool UpdateServerMap(QModbusDevice *server);
    void UpdateServerRegistersData();

    quint8 GetClientFunctionCode();
    QList<quint16> GetClientRegisterValue();
    QByteArray GetClientPdu();
    QTableView *GetTableView(QModbusDataUnit::RegisterType table);
    QList<quint16> GetTableValues(QTableView *table_view, int row, int count);

    void OutputModbusReply(QModbusReply *reply, int function_code);
    void OutputMessage(const QString &msg, bool isError,
                       const QString &color = QString(),
                       const QString &flag = QString());
    bool IsConnected();
    bool WriteSettingsArray(const QString &group, const QString &key,
                            const QString &value, int index, int max_index);
};

#endif // SAKMODBUSUI_H
