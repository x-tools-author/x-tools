/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>

#include "SAKModbusCommonSerialPortSection.hh"
#include "ui_SAKModbusCommonSerialPortSection.h"

SAKModbusCommonSerialPortSection::SAKModbusCommonSerialPortSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonSerialPortSection)
{
    ui->setupUi(this);

    // Add avalable ports to combo box
    auto ret = QSerialPortInfo::availablePorts();
    QStandardItemModel *itemModel = new QStandardItemModel(this);
    for (auto var : ret){
        auto item = new  QStandardItem(var.portName());
        itemModel->appendRow(item);
        item->setToolTip(var.description());
    }
    ui->nameComboBox->setModel(itemModel);

    // ------------------------------------------------------------------------
    auto comboBox = ui->dataBitsComboBox;
    comboBox->addItem("8", QVariant::fromValue(QSerialPort::Data8));
    comboBox->addItem("7", QVariant::fromValue(QSerialPort::Data7));
    comboBox->addItem("6", QVariant::fromValue(QSerialPort::Data6));
    comboBox->addItem("5", QVariant::fromValue(QSerialPort::Data5));

    // ------------------------------------------------------------------------
    comboBox = ui->stopBitsComboBox;
    comboBox->addItem("1", QVariant::fromValue(QSerialPort::OneStop));
#ifdef Q_OS_WINDOWS
    comboBox->addItem("1.5", QVariant::fromValue(QSerialPort::OneAndHalfStop));
#endif
    comboBox->addItem("2", QVariant::fromValue(QSerialPort::TwoStop));

    // ------------------------------------------------------------------------
    comboBox = ui->parityComboBox;
    comboBox->addItem(tr("No"), QVariant::fromValue(QSerialPort::NoParity));
    comboBox->addItem(tr("Even"), QVariant::fromValue(QSerialPort::EvenParity));
    comboBox->addItem(tr("Odd"), QVariant::fromValue(QSerialPort::OddParity));
    comboBox->addItem(tr("Space"), QVariant::fromValue(QSerialPort::SpaceParity));
    comboBox->addItem(tr("Mark"), QVariant::fromValue(QSerialPort::MarkParity));

    // ------------------------------------------------------------------------
    comboBox = ui->baudRateComboBox;
    QList<qint32> bd = QSerialPortInfo::standardBaudRates();
    for (auto var:bd) {
        comboBox->addItem(QString::number(var), QVariant::fromValue(var));
    }
    comboBox->setCurrentText("9600");
}

SAKModbusCommonSerialPortSection::~SAKModbusCommonSerialPortSection()
{
    delete ui;
}
