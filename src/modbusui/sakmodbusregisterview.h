/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSREGISTERVIEW_H
#define SAKMODBUSREGISTERVIEW_H

#include <QTimer>
#include <QWidget>
#include <QModbusDevice>
#include <QModbusClient>
#include <QModbusServer>
#include <QLoggingCategory>

namespace Ui {
class SAKModbusRegisterView;
}

class SAKModbusRegisterView : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusRegisterView(QWidget *parent = nullptr);
    ~SAKModbusRegisterView();

    void ResetModbusDevice(QModbusDevice *device);
    void SetUiVisible(bool is_client);
    void SetUiEnable(bool enable);

private:
    const QLoggingCategory logging_category_{"sak.modbus.registerview"};
    Ui::SAKModbusRegisterView *ui_;
    QModbusDevice *modbus_device_;
    QModbusClient *modbus_client_;
    QModbusServer *modbus_server_;
    QTimer *auto_send_timer_;

private:
    void ResetServer(QModbusDevice *device);
    void ResetClient(QModbusDevice *device);
    void SendRequest();
    void SendReadRequest();
    void SendWriteRequest();
    QList<quint16> GetValues();
    void SetValues(const QList<quint16> &values);
    void UpdateTableWidget();
    void UpdateValueFormat(int format);
    bool IsReadOperation(int function_code);
    bool IsWriteOperation(int function_code);

    void OnAutoSendComboBoxActived();
    void OnAutoSendTimerTimtout();
    void OnSendPushButtonClicked();
    void OnAddressChanged(int address);
    void OnQuantityChanged(int quantity);
    void OnTextFormatChanged(int format);
};

#endif // SAKMODBUSREGISTERVIEW_H
