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
class xToolsModbusStudioUi;
}

struct xToolsModbusUiSettingKeys;
class xToolsModbusStudioUi : public QWidget
{
    Q_OBJECT
public:
    explicit xToolsModbusStudioUi(QWidget *parent = Q_NULLPTR);
    ~xToolsModbusStudioUi();

signals:
    void deviceChanged(QModbusDevice *device);

private:
    Ui::xToolsModbusStudioUi *ui;
    QModbusDevice *m_modbusDevice{Q_NULLPTR};
    QSettings *m_settings{Q_NULLPTR};
    QStandardItemModel *m_registerModel{Q_NULLPTR};
    xToolsModbusUiSettingKeys *m_keyCtx;

private:
    void initComponents();
    void initComponentDevices();
    void initComponentAddress();
    void initComponentPortName();
    void initComponnetBaudRate();
    void initComponnetDataBits();
    void initComponnetStopBits();
    void initComponnetParity();
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

    void initSignals();
    void initSignalsDevice();
    void initSignalsNetworking();
    void initSignalsSerialPort();
    void initSignalsClient();
    void initSignalsServer();
    void initSignalsClientOperations();

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

private:
    QModbusDevice *CreateModbusDevice();
    QTableView *CreateTableView(int rowCount, QTableView *tableView);

    void updateUiState(bool connected);
    void updateClientTableView();
    void updateClientTableViewData(const QList<quint16> &values);
    void updateClientReadWriteButtonState();
    void updateClientParameters();
    void updateClientTableViewAddress(QTableView *view, int startAddress);
    void updateServerParameters();
    bool updateServerMap(QModbusDevice *server);
    void updateServerRegistersData();

    quint8 getClientFunctionCode();
    QList<quint16> getClientRegisterValue();
    QByteArray getClientPdu();
    QTableView *getTableView(QModbusDataUnit::RegisterType table);
    QList<quint16> getTableValues(QTableView *tableView, int row, int count);

    void outputModbusReply(QModbusReply *reply, int functionCode);
    void outputMessage(const QString &msg,
                       bool isError,
                       const QString &color = QString(),
                       const QString &flag = QString());
    bool isConnected();
    bool writeSettingsArray(
        const QString &group, const QString &key, const QString &value, int index, int maxIndex);
};
