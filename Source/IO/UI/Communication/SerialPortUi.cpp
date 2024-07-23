/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPortUi.h"
#include "ui_SerialPortUi.h"

#include <QSerialPort>
#include <QSerialPortInfo>

SerialPortUi::SerialPortUi(xIO::CommunicationType type, QWidget *parent)
    : CommunicationUi(type, parent)
    , ui(new Ui::SerialPortUi)
{
    ui->setupUi(this);

    refresh();

    QList<int> baudRates = QSerialPortInfo::standardBaudRates();
    for (const auto &baudRate : baudRates) {
        ui->comboBoxBaudRate->addItem(QString::number(baudRate));
    }
    ui->comboBoxBaudRate->setCurrentText("9600");

    ui->comboBoxDataBits->addItem("8", QSerialPort::Data8);
    ui->comboBoxDataBits->addItem("7", QSerialPort::Data7);
    ui->comboBoxDataBits->addItem("6", QSerialPort::Data6);
    ui->comboBoxDataBits->addItem("5", QSerialPort::Data5);

    ui->comboBoxParity->addItem(tr("None"), QSerialPort::NoParity);
    ui->comboBoxParity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->comboBoxParity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->comboBoxParity->addItem(tr("Space"), QSerialPort::SpaceParity);
    ui->comboBoxParity->addItem(tr("Mark"), QSerialPort::MarkParity);

    ui->comboBoxStopBits->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->comboBoxStopBits->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->comboBoxStopBits->addItem("2", QSerialPort::TwoStop);

    ui->comboBoxFlowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
#if 0
    ui->comboBoxFlowControl->addItem(tr("RTS/CTS(Hardware)"), QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem(tr("XON/XOFF(Software)"), QSerialPort::SoftwareControl);
#else
    ui->comboBoxFlowControl->addItem(tr("Hardware"), QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem(tr("Software"), QSerialPort::SoftwareControl);
#endif
}

QVariantMap SerialPortUi::save() const
{
    QVariantMap map;
    map["portName"] = ui->comboBoxPortName->currentText();
    map["baudRate"] = ui->comboBoxBaudRate->currentText().toInt();
    map["dataBits"] = ui->comboBoxDataBits->currentData().toInt();
    map["parity"] = ui->comboBoxParity->currentData().toInt();
    map["stopBits"] = ui->comboBoxStopBits->currentData().toInt();
    map["flowControl"] = ui->comboBoxFlowControl->currentData().toInt();
    return map;
}

void SerialPortUi::load(const QVariantMap &map)
{
    if (map.isEmpty()) {
        return;
    }

    QString portName = map.value("portName").toString();
    int baudRate = map.value("baudRate").toInt();
    int dataBits = map.value("dataBits").toInt();
    int parity = map.value("parity").toInt();
    int stopBits = map.value("stopBits").toInt();
    int flowControl = map.value("flowControl").toInt();

    ui->comboBoxPortName->setCurrentText(portName);
    ui->comboBoxBaudRate->setCurrentText(QString::number(baudRate));
    ui->comboBoxDataBits->setCurrentIndex(ui->comboBoxDataBits->findData(dataBits));
    ui->comboBoxParity->setCurrentIndex(ui->comboBoxParity->findData(parity));
    ui->comboBoxStopBits->setCurrentIndex(ui->comboBoxStopBits->findData(stopBits));
    ui->comboBoxFlowControl->setCurrentIndex(ui->comboBoxFlowControl->findData(flowControl));
}

void SerialPortUi::refresh()
{
    ui->comboBoxPortName->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (const auto &info : infos) {
        ui->comboBoxPortName->addItem(info.portName());
    }
}
