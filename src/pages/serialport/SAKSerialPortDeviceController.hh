/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKSERIALPORTDEVICECONTROLLER_HH
#define SAKSERIALPORTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

namespace Ui {
    class SAKSerialPortDeviceController;
}

class SAKSerialPortDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKSerialPortDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKSerialPortDeviceController();

    enum QSerialPort::DataBits dataBits();
    enum QSerialPort::StopBits stopBits();
    enum QSerialPort::Parity parity();
    enum QSerialPort::FlowControl flowControl();
    QString name();
    qint32 baudRate();

    void refresh();
    void setUiEnable(bool enable);
private:
    QMutex uiMutex;
private:
    Ui::SAKSerialPortDeviceController *ui;

    QComboBox *serialportsComboBox;
    QComboBox *baudrateComboBox;
    QComboBox *databitsComboBox;
    QComboBox *stopbitsComboBox;
    QComboBox *parityComboBox;
    QComboBox *flowControlComboBox;
    QCheckBox *customBaudrateCheckBox;
private slots:
    void on_customBaudrateCheckBox_clicked();
};

#endif
