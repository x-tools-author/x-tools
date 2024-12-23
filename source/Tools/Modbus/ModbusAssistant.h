/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonArray>
#include <QLoggingCategory>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusReply>
#include <QModbusServer>
#include <QSettings>
#include <QStandardItemModel>
#include <QTableView>
#include <QVector>
#include <QWidget>

namespace Ui {
class ModbusAssistant;
}

struct ModbusSettingKeys;
class ModbusAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ModbusAssistant(QWidget *parent = Q_NULLPTR);
    ~ModbusAssistant();

signals:
    void deviceChanged(QModbusDevice *device);

private:
    Ui::ModbusAssistant *ui;
    QModbusDevice *m_modbusDevice{Q_NULLPTR};
    QStandardItemModel *m_clientRegisterModel{Q_NULLPTR};
    ModbusSettingKeys *m_keyCtx;
    QTableView *m_clientView{nullptr};
    int m_textFormat{0};
    QSettings *m_settings;

private:
    void initComponents();
    void initComponentDevices();
    void initComponentAddress();
    void initComponentPortName();
    void initComponentBaudRate();
    void initComponentDataBits();
    void initComponentStopBits();
    void initComponentParity();
    void initComponentFunctionCode();
    void initComponentRegisterTableView();
    void initComponentInput();
    void initComponentRegisterTabWidget();

    void initSettings();
    void initSettingsDevice();
    void initSettingsNetwork();
    void initSettingsSerialPort();
    void initSettingsClient();
    void initSettingsServer();
    void initSettingsClientOperations();
    void initSettingsInput();
    void initSettingsInputControl();

    void initSignals();
    void initSignalsDevice();
    void initSignalsNetworking();
    void initSignalsSerialPort();
    void initSignalsClient();
    void initSignalsServer();
    void initSignalsClientOperations();
    void initSignalsInput();
    void initSignalsInputControl();

    void onErrorOccurred();
    void onDeviceTypeChanged();
    void onCloseClicked();
    void onOpenClicked();
    void onAddressChanged();
    void onPortChanged();
    void onCustomAddressChanged();
    void onPortNameChanged();
    void onParityChanged();
    void onBaudRateChanged();
    void onDataBitsChanged();
    void onStopBistChanged();
    void onInvokeRefresh();
    void onClientTimeoutChanged();
    void onClientRepeatTimeChanged();
    void onServerIsBusyChanged();
    void onServerJustListenChanged();
    void onServerAddressChanged();
    void onFunctionCodeChanged();
    void onTargetAddressChanged();
    void onStartAddressChanged();
    void onAddressNumberChanged();

    void onReadClicked();
    void onWriteClicked();
    void onSendClicked();

    void onDateWritten(QModbusDataUnit::RegisterType table, int address, int size);
    void onItemChanged(QStandardItem *item);
    void onInputFormatChanged();

private:
    QModbusDevice *createModbusDevice();
    QTableView *createTableView(int rowCount, QTableView *tableView);

    void updateUiState(bool connected);
    void updateClientTableView(int currentFormat, int targetFormat);
    void updateClientTableViewData(int currentFormat, int targetFormat);
    void updateClientTableViewData(const QList<quint16> &values);
    void updateClientTableViewAddress(QTableView *view, int startAddress);
    void updateClientReadWriteButtonState();
    void updateClientParameters();
    void updateServerParameters();
    bool updateServerMap(QModbusDevice *server);
    void updateServerRegistersViews(int currentFormat, int targetFormat);

    quint8 getClientFunctionCode();
    QList<quint16> getClientRegisterValue();
    QByteArray getClientPdu();
    QTableView *getTableView(QModbusDataUnit::RegisterType table);
    QList<quint16> getTableValues(QTableView *tableView, int row, int count);

    // clang-format off
    void outputModbusReply(QModbusReply *reply, int functionCode);
    void outputMessage(const QString &msg, bool isError, const QString &color = QString(), const QString &flag = QString());
    bool isConnected();
    bool writeSettingsArray(const QString &group, const QString &key, const QString &value, int index, int maxIndex);
    int startAddress();
    // clang-format on
};
