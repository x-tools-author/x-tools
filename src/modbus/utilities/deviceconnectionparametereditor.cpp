/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "deviceconnectionparametereditor.h"
#include "ui_deviceconnectionparametereditor.h"

#include <QJsonObject>

#include "../common/xmodbuscommon.h"

namespace xModbus {

DeviceConnectionParameterEditor::DeviceConnectionParameterEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeviceConnectionParameterEditor)
{
    ui->setupUi(this);

    setupModebusDeviceType(ui->comboBoxDeviceType);
    setupRtuNames(ui->comboBoxRtuNames);
    setupRtuDataBits(ui->comboBoxRtuDataBits);
    setupRtuParity(ui->comboBoxRtuParity);
    setupRtuStopBits(ui->comboBoxRtuStopBits);
    setupRtuBaudRate(ui->comboBoxRtuBaudRate);
}

DeviceConnectionParameterEditor::~DeviceConnectionParameterEditor()
{
    delete ui;
}

void DeviceConnectionParameterEditor::load(const QJsonObject &parameters)
{
    DeviceConnectionParameters params = json2DeviceConnectionParameters(parameters);

    ui->lineEditDeviceName->setText(params.deviceName);
    ui->comboBoxDeviceType->setCurrentIndex(ui->comboBoxDeviceType->findData(params.deviceType));
    ui->comboBoxRtuNames->setCurrentText(params.portName);
    ui->comboBoxRtuDataBits->setCurrentIndex(ui->comboBoxRtuDataBits->findData(params.dataBits));
    ui->comboBoxRtuParity->setCurrentIndex(ui->comboBoxRtuParity->findData(params.parity));
    ui->comboBoxRtuStopBits->setCurrentIndex(ui->comboBoxRtuStopBits->findData(params.stopBits));
    ui->comboBoxRtuBaudRate->setCurrentIndex(ui->comboBoxRtuBaudRate->findData(params.baudRate));
    ui->comboBoxTcpAddress->setCurrentText(params.tcpAddress);
    ui->spinBoxTcpPort->setValue(params.tcpPort);
    ui->spinBoxNumberOfRetries->setValue(params.numberOfRetries);
    ui->spinBoxTimeout->setValue(params.timeout);
    ui->spinBoxServerAddress->setValue(params.serverAddress);
    ui->checkBoxListenOnlyMode->setChecked(params.listenOnlyMode);
}

QJsonObject DeviceConnectionParameterEditor::save() const
{
    DeviceConnectionParameters parameters;
    parameters.deviceName = ui->lineEditDeviceName->text();
    parameters.deviceType = ui->comboBoxDeviceType->currentData().toInt();
    parameters.portName = ui->comboBoxRtuNames->currentText();
    parameters.dataBits = ui->comboBoxRtuDataBits->currentData().toInt();
    parameters.parity = ui->comboBoxRtuParity->currentData().toInt();
    parameters.stopBits = ui->comboBoxRtuStopBits->currentData().toInt();
    parameters.baudRate = ui->comboBoxRtuBaudRate->currentData().toInt();
    parameters.tcpAddress = ui->comboBoxTcpAddress->currentText();
    parameters.tcpPort = ui->spinBoxTcpPort->value();
    parameters.numberOfRetries = ui->spinBoxNumberOfRetries->value();
    parameters.timeout = ui->spinBoxTimeout->value();
    parameters.serverAddress = ui->spinBoxServerAddress->value();
    parameters.listenOnlyMode = ui->checkBoxListenOnlyMode->isChecked();

    return deviceConnectionParameters2Json(parameters);
}

} // namespace xModbus