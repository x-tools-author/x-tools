/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
