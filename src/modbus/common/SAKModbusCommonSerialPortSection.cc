/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>

#include "SAKCommonInterface.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "ui_SAKModbusCommonSerialPortSection.h"

SAKModbusCommonSerialPortSection::SAKModbusCommonSerialPortSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonSerialPortSection)
{
    ui->setupUi(this);
    SAKCommonInterface::addSerialPortNametItemsToComboBox(ui->nameComboBox);
    SAKCommonInterface::addSerialPortDataBitItemsToComboBox(ui->dataBitsComboBox);
    SAKCommonInterface::addSerialPortStopBitItemsToComboBox(ui->stopBitsComboBox);
    SAKCommonInterface::addSerialPortParityItemsToComboBox(ui->parityComboBox);
    SAKCommonInterface::addSerialPortBaudRateItemsToComboBox(ui->baudRateComboBox);
}

SAKModbusCommonSerialPortSection::~SAKModbusCommonSerialPortSection()
{
    delete ui;
}

SAKModbusCommonSerialPortSection::ParametersContext SAKModbusCommonSerialPortSection::parametersContext()
{
    ParametersContext paras;
    paras.portName = ui->nameComboBox->currentText();
    paras.parity = ui->parityComboBox->currentData().toInt();
    paras.baudRate = ui->baudRateComboBox->currentData().toInt();
    paras.dataBits = ui->dataBitsComboBox->currentData().toInt();
    paras.stopBits = ui->stopBitsComboBox->currentData().toInt();
#ifdef QT_DEBUG
    qDebug() << "name:" << paras.portName
             << "parity:" << paras.parity
             << "baudRate:" << paras.baudRate
             << "dataBits:" << paras.dataBits
             << "stopBits" << paras.stopBits;
#endif
    return paras;
}

void SAKModbusCommonSerialPortSection::initModbusDeviceParamerers(QModbusDevice *dev)
{
    if (dev){
        auto seruialPortParasCtx = parametersContext();
        dev->setConnectionParameter(QModbusDevice::SerialPortNameParameter, seruialPortParasCtx.portName);
        dev->setConnectionParameter(QModbusDevice::SerialParityParameter, seruialPortParasCtx.parity);
        dev->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, seruialPortParasCtx.baudRate);
        dev->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, seruialPortParasCtx.dataBits);
        dev->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, seruialPortParasCtx.stopBits);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Parameters can not be null!");
    }
}
