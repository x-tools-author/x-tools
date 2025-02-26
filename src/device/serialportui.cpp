/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialportui.h"
#include "ui_serialportui.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "devicepage/common/xio.h"

namespace xTools {

SerialPortUi::SerialPortUi(QWidget *parent)
    : CommunicationUi(xIO::CommunicationType::SerialPort, parent)
    , ui(new Ui::SerialPortUi)
{
    ui->setupUi(this);
#if defined(Q_OS_LINUX)
    ui->comboBoxPortName->setEditable(true);
#endif

    refresh();

    xIO::setupBaudRate(ui->comboBoxBaudRate);
    xIO::setupDataBits(ui->comboBoxDataBits);
    xIO::setupParity(ui->comboBoxParity);
    xIO::setupStopBits(ui->comboBoxStopBits);
    xIO::setupFlowControl(ui->comboBoxFlowControl);
}

QVariantMap SerialPortUi::save() const
{
    QVariantMap map;
    xIO::SerialPortItemKeys keys;
    map[keys.portName] = ui->comboBoxPortName->currentText();
    map[keys.baudRate] = ui->comboBoxBaudRate->currentText().toInt();
    map[keys.dataBits] = ui->comboBoxDataBits->currentData().toInt();
    map[keys.parity] = ui->comboBoxParity->currentData().toInt();
    map[keys.stopBits] = ui->comboBoxStopBits->currentData().toInt();
    map[keys.flowControl] = ui->comboBoxFlowControl->currentData().toInt();
    return map;
}

void SerialPortUi::load(const QVariantMap &map)
{
    if (map.isEmpty()) {
        return;
    }

    xIO::SerialPortItemKeys keys;
    QString portName = map.value(keys.portName).toString();
    int baudRate = map.value(keys.baudRate).toInt();
    int dataBits = map.value(keys.dataBits).toInt();
    int parity = map.value(keys.parity).toInt();
    int stopBits = map.value(keys.stopBits).toInt();
    int flowControl = map.value(keys.flowControl).toInt();

    ui->comboBoxPortName->setCurrentText(portName);
    ui->comboBoxBaudRate->setCurrentText(QString::number(baudRate));
    ui->comboBoxDataBits->setCurrentIndex(ui->comboBoxDataBits->findData(dataBits));
    ui->comboBoxParity->setCurrentIndex(ui->comboBoxParity->findData(parity));
    ui->comboBoxStopBits->setCurrentIndex(ui->comboBoxStopBits->findData(stopBits));
    ui->comboBoxFlowControl->setCurrentIndex(ui->comboBoxFlowControl->findData(flowControl));
}

void SerialPortUi::refresh()
{
    xIO::setupPortName(ui->comboBoxPortName);
}

} // namespace xTools
