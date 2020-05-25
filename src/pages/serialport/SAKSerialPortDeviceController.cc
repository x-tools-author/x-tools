/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKGlobal.hh"
#include "SAKSerialPortDeviceController.hh"
#include "ui_SAKSerialPortDeviceController.h"
SAKSerialPortDeviceController::SAKSerialPortDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKSerialPortDeviceController)
    ,serialportsComboBox(Q_NULLPTR)
    ,baudrateComboBox(Q_NULLPTR)
    ,databitsComboBox(Q_NULLPTR)
    ,stopbitsComboBox(Q_NULLPTR)
    ,parityComboBox(Q_NULLPTR)
    ,flowControlComboBox(Q_NULLPTR)
    ,customBaudrateCheckBox(Q_NULLPTR)
{
    ui->setupUi(this);

    serialportsComboBox      = ui->serialportsComboBox;
    baudrateComboBox         = ui->baudrateComboBox;
    databitsComboBox         = ui->databitsComboBox;
    stopbitsComboBox         = ui->stopbitsComboBox;
    parityComboBox           = ui->parityComboBox;
    flowControlComboBox      = ui->flowControlComboBox;
    customBaudrateCheckBox   = ui->customBaudrateCheckBox;

    refresh();
}

SAKSerialPortDeviceController::~SAKSerialPortDeviceController()
{
    delete ui;
}

void SAKSerialPortDeviceController::refresh()
{
    SAKGlobal::initComComboBox(serialportsComboBox);
    SAKGlobal::initBaudRateComboBox(baudrateComboBox);
    SAKGlobal::initDataBitsComboBox(databitsComboBox);
    SAKGlobal::initStopBitsComboBox(stopbitsComboBox);
    SAKGlobal::initParityComboBox(parityComboBox);
    SAKGlobal::initFlowControlComboBox(flowControlComboBox);
}

void SAKSerialPortDeviceController::setUiEnable(bool enable)
{
    serialportsComboBox->setEnabled(enable);
    baudrateComboBox->setEnabled(enable);
    databitsComboBox->setEnabled(enable);
    stopbitsComboBox->setEnabled(enable);
    parityComboBox->setEnabled(enable);
    customBaudrateCheckBox->setEnabled(enable);
    flowControlComboBox->setEnabled(enable);
}

enum QSerialPort::DataBits SAKSerialPortDeviceController::dataBits()
{
    return static_cast<QSerialPort::DataBits>(databitsComboBox->currentData().toInt());
}

enum QSerialPort::StopBits SAKSerialPortDeviceController::stopBits()
{
    return static_cast<QSerialPort::StopBits>(stopbitsComboBox->currentData().toInt());
}

enum QSerialPort::Parity SAKSerialPortDeviceController::parity()
{
    return static_cast<QSerialPort::Parity>(parityComboBox->currentData().toInt());
}

QString SAKSerialPortDeviceController::name()
{
    QString portName = serialportsComboBox->currentText();
    portName = portName.split(' ').first();
    return  portName;
}

qint32 SAKSerialPortDeviceController::baudRate()
{
    qint32 rate = 9600;
    bool ok = false;
    if (baudrateComboBox->currentText().isEmpty()){
        return 9600;
    }

    rate = baudrateComboBox->currentText().toInt(&ok);
    if (!ok){
        rate = 9600;
    }

    return rate;
}

void SAKSerialPortDeviceController::on_customBaudrateCheckBox_clicked()
{
    if (customBaudrateCheckBox->isChecked()){
        baudrateComboBox->setEditable(true);
        baudrateComboBox->lineEdit()->selectAll();
        baudrateComboBox->lineEdit()->setFocus();
    }else{
        baudrateComboBox->setEditable(false);
    }
}
