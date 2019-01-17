/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SerialportSAKIODeviceControler.h"

#include "ui_SerialportSAKIODeviceControler.h"

#include <QSerialPortInfo>
#include <QApplication>

SerialportSAKIODeviceControler::SerialportSAKIODeviceControler(QWidget *parent)
    :SAKIODeviceControler(parent)
    ,ui(new Ui::SerialportSAKIODeviceControler)
{
    setObjectName(QString("SerialPortController"));
    ui->setupUi(this);
    initUI();
}

SerialportSAKIODeviceControler::~SerialportSAKIODeviceControler()
{

}

void SerialportSAKIODeviceControler::open()
{
    QString portName = ui->comboBoxSerialports->currentText().split(' ').first();
    QString baudRate = ui->comboBoxBaudrate->currentText();
    QString dataBits = ui->comboBoxDatabits->currentText();
    QString stopBits = ui->comboBoxStopbits->currentText();
    QString parity   = ui->comboBoxParity->currentText();

    emit need2open(portName, baudRate, dataBits, stopBits, parity);
}

void SerialportSAKIODeviceControler::afterDeviceOpen()
{
    ui->comboBoxSerialports->setEnabled(false);
    ui->comboBoxBaudrate->setEnabled(false);
    ui->comboBoxDatabits->setEnabled(false);
    ui->comboBoxStopbits->setEnabled(false);
    ui->comboBoxParity->setEnabled(false);
}

void SerialportSAKIODeviceControler::afterDeviceClose()
{
    ui->comboBoxSerialports->setEnabled(true);
    ui->comboBoxBaudrate->setEnabled(true);
    ui->comboBoxDatabits->setEnabled(true);
    ui->comboBoxStopbits->setEnabled(true);
    ui->comboBoxParity->setEnabled(true);
}

void SerialportSAKIODeviceControler::refresh()
{
    QList <QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    ui->comboBoxSerialports->clear();
    if (infoList.isEmpty()){
        ui->comboBoxSerialports->addItem(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning), tr("未检测到串口设备"));
    }else {
        foreach(QSerialPortInfo info, infoList){
            ui->comboBoxSerialports->addItem(info.portName() + " " + info.description());
        }
    }
}

void SerialportSAKIODeviceControler::initUI()
{
    refresh();

    QList <qint32> baudRateList = QSerialPortInfo::standardBaudRates();
    ui->comboBoxBaudrate->clear();
    foreach (qint32 baudRate, baudRateList) {
        ui->comboBoxBaudrate->addItem(QString::number(baudRate));
    }
    ui->comboBoxBaudrate->setCurrentText("9600");

    QStringList dataBitsList;
    dataBitsList << "8" << "7" << "6" << "5";
    ui->comboBoxDatabits->addItems(dataBitsList);


    QStringList stopBitsList;
    stopBitsList << "1";
#ifdef Q_OS_WIN
    stopBitsList << "1.5";
#endif
    stopBitsList << "2";
    ui->comboBoxStopbits->addItems(stopBitsList);

    QStringList parityList;
    parityList << "NoParity" << "EvenParity" << "OddParity" << "SpaceParity" << "MarkParity";
    ui->comboBoxParity->addItems(parityList);
}
