/******************************************************************************
 * Copyright 2022 Qter(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSSTUDIO_H
#define SAKMODBUSSTUDIO_H

#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QTableView>
#include <QJsonArray>
#include <QModbusReply>
#include <QModbusDevice>
#include <QModbusDataUnit>
#include <QLoggingCategory>
#include <QStandardItemModel>

namespace Ui {
    class SAKModbusStudio;
};

class QSqlDatabase;
class SAKModbusStudio : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusStudio(QSettings *settings, QWidget *parent = Q_NULLPTR);
    Q_INVOKABLE SAKModbusStudio(QSettings *settings,
                          const QString settingsGroup,
                          QSqlDatabase *sqlDatabase = Q_NULLPTR,
                          QWidget *parent = Q_NULLPTR);
    ~SAKModbusStudio();
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
    Ui::SAKModbusStudio *ui;
    QModbusDevice *mModbusDevice;
    const QLoggingCategory mLoggingCategory;
    QSettings *mSettings;
    QStandardItemModel *mRegisterModel;
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
    void onInvokeClose();
    void onInvokeOpen();
    void onAddressChanged();
    void onPortChanged();
    void onCustomAddressChanged();
    void onPortNameChanged();
    void onParityChanged();
    void onBaudRateChanged();
    void onDataBitsChanged();
    void onStopBistChanged();
    void onInvokeRefresh();
    void onCustomBaudRateChanged();
    void onClientTimeoutChanged();
    void onClientRepeatTimeChanged();
    void onServerIsBusyChanged();
    void onServerJustListenChanged();
    void onServerAddressChanged();
    void onFunctionCodeChanged();
    void onTargetAddressChanged();
    void onStartAddressChanged();
    void onAddressNumberChanged();
private:
    void updateUiState(bool opened);
    quint16 calculateModbusCrc(const QByteArray &data);
    quint16 cookedRegisterString(QString text, int base);
    void synchronizationRegister(QModbusDevice *server);
    bool writeSettingsArray(const QString &group, const QString &key,
                            const QString &value, int index, int maxIndex);
    QModbusDataUnit::RegisterType registerType(int type);

    // The interfaces of modbus client operations.
    void clientRead();
    void clientWrite();
    void clientSend();
    void clientUpdateTable();
    void clientSetRegisterValue(QJsonArray values);
    void clientUpdateRWBtState();
    void clientUpdateParameters();
    quint8 clientFunctionCode();
    QJsonArray clientRegisterValue();
    QByteArray clientPdu();

    // Show message box.
    void showMessageBox(const QString &title, const QString &msg);
    void showMessageBoxDeviceIsNotReady();

    // Output info to ui.
    void outputModbusReply(QModbusReply *reply, int functionCode);
    void outputModbusRequestSend(int serverAddress, const QByteArray &pdu);
    void outputModbusRequestRead(int serverAddress, int functionCode,
                                 int startAddress, int addressNumber);
    void outputModbusRequestWrite(int serverAddress, int functionCode,
                                  int startAddress, int addressNumber,
                                  QJsonArray values);
    void outputMessage(const QString &msg, bool isError,
                       const QString &color = QString(),
                       const QString &flag = QString());

    // Table view operations.
    QTableView *tableView(QModbusDataUnit::RegisterType table);
    QTableView *tableViewInit(int rowCount, QTableView *tv);
    QVector<quint16> tableValues(QTableView *tv, int row, int count);
    void tableViewUpdateAddress(QTableView *tv, int startAddress);
    void tableViewUpdateRow(QStandardItem *item);
    void tableViewUpdateData(int table, int address, int size);
};

#endif // SAKMODBUSSTUDIO_H
