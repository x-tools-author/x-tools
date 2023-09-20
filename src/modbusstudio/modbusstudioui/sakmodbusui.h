/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKMODBUSUI_H
#define SAKMODBUSUI_H

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
class SAKModbusUi;
}

struct SAKModbusUiSettingKeys;
class SAKModbusUi : public QWidget {
  Q_OBJECT
 public:
  explicit SAKModbusUi(QWidget *parent = Q_NULLPTR);
  ~SAKModbusUi();

 signals:
  void deviceChanged(QModbusDevice *device);

 private:
  Ui::SAKModbusUi *ui_;
  QModbusDevice *modbus_device_{Q_NULLPTR};
  QSettings *settings_{Q_NULLPTR};
  QStandardItemModel *register_model_{Q_NULLPTR};
  const QLoggingCategory kLoggingCategory{"SAK.Modbus"};
  SAKModbusUiSettingKeys *key_ctx_;

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

  void OnDateWritten(QModbusDataUnit::RegisterType table, int address,
                     int size);
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

#endif  // SAKMODBUSUI_H
