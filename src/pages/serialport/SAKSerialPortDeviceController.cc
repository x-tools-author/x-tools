/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    uiMutex.lock();
    SAKGlobal::initComComboBox(serialportsComboBox);
    SAKGlobal::initBaudRateComboBox(baudrateComboBox);
    SAKGlobal::initDataBitsComboBox(databitsComboBox);
    SAKGlobal::initStopBitsComboBox(stopbitsComboBox);
    SAKGlobal::initParityComboBox(parityComboBox);
    SAKGlobal::initFlowControlComboBox(flowControlComboBox);
    uiMutex.unlock();
}

void SAKSerialPortDeviceController::setUiEnable(bool enable)
{
    uiMutex.lock();
    serialportsComboBox->setEnabled(enable);
    baudrateComboBox->setEnabled(enable);
    databitsComboBox->setEnabled(enable);
    stopbitsComboBox->setEnabled(enable);
    parityComboBox->setEnabled(enable);
    customBaudrateCheckBox->setEnabled(enable);
    flowControlComboBox->setEnabled(enable);
    uiMutex.unlock();
}

enum QSerialPort::DataBits SAKSerialPortDeviceController::dataBits()
{
    uiMutex.lock();
    QSerialPort::DataBits ret = static_cast<QSerialPort::DataBits>(databitsComboBox->currentData().toInt());
    uiMutex.unlock();
    return ret;
}

enum QSerialPort::StopBits SAKSerialPortDeviceController::stopBits()
{
    uiMutex.lock();
    QSerialPort::StopBits ret = static_cast<QSerialPort::StopBits>(stopbitsComboBox->currentData().toInt());
    uiMutex.unlock();
    return ret;
}

enum QSerialPort::Parity SAKSerialPortDeviceController::parity()
{
    uiMutex.lock();
    QSerialPort::Parity ret = static_cast<QSerialPort::Parity>(parityComboBox->currentData().toInt());
    uiMutex.unlock();
    return ret;
}

enum QSerialPort::FlowControl SAKSerialPortDeviceController::flowControl()
{
    uiMutex.lock();
    QSerialPort::FlowControl ret = static_cast<QSerialPort::FlowControl>(flowControlComboBox->currentData().toInt());
    uiMutex.unlock();
    return ret;
}

QString SAKSerialPortDeviceController::name()
{
    uiMutex.lock();
    QString portName = serialportsComboBox->currentText();
    portName = portName.split(' ').first();
    uiMutex.unlock();
    return  portName;
}

qint32 SAKSerialPortDeviceController::baudRate()
{
    uiMutex.lock();
    qint32 rate = 9600;
    bool ok = false;
    if (baudrateComboBox->currentText().isEmpty()){
        return 9600;
    }

    rate = baudrateComboBox->currentText().toInt(&ok);
    if (!ok){
        rate = 9600;
    }
    uiMutex.unlock();

    return rate;
}

void SAKSerialPortDeviceController::on_customBaudrateCheckBox_clicked()
{
    uiMutex.lock();
    if (customBaudrateCheckBox->isChecked()){
        baudrateComboBox->setEditable(true);
        baudrateComboBox->lineEdit()->selectAll();
        baudrateComboBox->lineEdit()->setFocus();
    }else{
        baudrateComboBox->setEditable(false);
    }
    uiMutex.unlock();
}
