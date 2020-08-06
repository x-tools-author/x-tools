/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTESTDEVICECONTROLLER_HH
#define SAKTESTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

namespace Ui {
    class SAKTestDeviceController;
}

class SAKTestDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKTestDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKTestDeviceController();

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
    Ui::SAKTestDeviceController *ui;

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
