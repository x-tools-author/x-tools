/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKSerialportAssistantController.hh"
#include "ui_SAKSerialportAssistantController.h"
SAKSerialportAssistantController::SAKSerialportAssistantController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKSerialportAssistantController)
{
    ui->setupUi(this);

    comboBoxSerialports      = ui->comboBoxSerialports;
    comboBoxBaudrate         = ui->comboBoxBaudrate;
    comboBoxDatabits         = ui->comboBoxDatabits;
    comboBoxStopbits         = ui->comboBoxStopbits;
    comboBoxParity           = ui->comboBoxParity;
    checkBoxCustomBaudrate   = ui->checkBoxCustomBaudrate;

    refresh();
}

SAKSerialportAssistantController::~SAKSerialportAssistantController()
{
    delete ui;
}

void SAKSerialportAssistantController::refresh()
{
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    comboBoxSerialports->clear();
    for (QSerialPortInfo info:serialPortInfos){
        comboBoxSerialports->addItem(info.portName() + " " + info.description());
    }
    if (serialPortInfos.isEmpty()){
        comboBoxSerialports->addItem(tr("未检测到串口设备"));
    }

    QList<qint32> standarBuadRates = QSerialPortInfo::standardBaudRates();
    comboBoxBaudrate->clear();
    for(qint32 baudRate:standarBuadRates){
        comboBoxBaudrate->addItem(QString::number(baudRate));
    }
    comboBoxBaudrate->setCurrentIndex(6);

    QMetaEnum dataBits = QMetaEnum::fromType<QSerialPort::DataBits>();
    comboBoxDatabits->clear();
    for (int i = 0; i < dataBits.keyCount(); i++){
        comboBoxDatabits->addItem(dataBits.valueToKey(dataBits.value(i)));
    }
    comboBoxDatabits->setCurrentIndex(3);

    QMetaEnum stopBitsEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    comboBoxStopbits->clear();
    for (int i = 0; i < stopBitsEnum.keyCount(); i++){
        comboBoxStopbits->addItem(stopBitsEnum.valueToKey(stopBitsEnum.value(i)));
    }
    comboBoxStopbits->setCurrentIndex(0);

    QMetaEnum parotyEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    comboBoxParity->clear();
    for (int i = 0; i < stopBitsEnum.keyCount(); i++){
        comboBoxParity->addItem(parotyEnum.valueToKey(parotyEnum.value(i)));
    }
    comboBoxParity->setCurrentIndex(0);
}

void SAKSerialportAssistantController::setUiEnable(bool enable)
{
    comboBoxSerialports->setEnabled(enable);
    comboBoxBaudrate->setEnabled(enable);
    comboBoxDatabits->setEnabled(enable);
    comboBoxStopbits->setEnabled(enable);
    comboBoxParity->setEnabled(enable);
    checkBoxCustomBaudrate->setEnabled(enable);
}

enum QSerialPort::DataBits SAKSerialportAssistantController::dataBits()
{
    QMetaEnum ret = QMetaEnum::fromType<QSerialPort::DataBits>();
    bool ok = false;
    int val = ret.keyToValue(comboBoxDatabits->currentText().toLatin1().data(), &ok);
    if (!ok){
        val = QSerialPort::Data8;
    }

    return static_cast<QSerialPort::DataBits>(val);
}

enum QSerialPort::StopBits SAKSerialportAssistantController::stopBits()
{
    QMetaEnum ret = QMetaEnum::fromType<QSerialPort::StopBits>();
    bool ok = false;
    int val = ret.keyToValue(comboBoxDatabits->currentText().toLatin1().data(), &ok);
    if (!ok){
        val = QSerialPort::OneStop;
    }

    return static_cast<QSerialPort::StopBits>(val);
}

enum QSerialPort::Parity SAKSerialportAssistantController::parity()
{
    QMetaEnum ret = QMetaEnum::fromType<QSerialPort::Parity>();
    bool ok = false;
    int val = ret.keyToValue(comboBoxParity->currentText().toLatin1().data(), &ok);
    if (!ok){
        val = QSerialPort::NoParity;
    }

    return static_cast<QSerialPort::Parity>(val);
}

QString SAKSerialportAssistantController::name()
{
    QString portName = comboBoxSerialports->currentText();
    portName = portName.split(' ').first();
    return  portName;
}

qint32 SAKSerialportAssistantController::baudRate()
{
    qint32 rate = 9600;
    bool ok = false;
    if (comboBoxBaudrate->currentText().isEmpty()){
        return 9600;
    }

    rate = comboBoxBaudrate->currentText().toInt(&ok);
    if (!ok){
        rate = 9600;
    }

    return rate;
}

void SAKSerialportAssistantController::on_checkBoxCustomBaudrate_clicked()
{
    if (checkBoxCustomBaudrate->isChecked()){
        comboBoxBaudrate->setEditable(true);
        comboBoxBaudrate->lineEdit()->selectAll();
        comboBoxBaudrate->lineEdit()->setFocus();
    }else{
        comboBoxBaudrate->setEditable(false);
    }
}
