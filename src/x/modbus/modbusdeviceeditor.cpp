/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "deviceeditor.h"
#include "ui_deviceeditor.h"

#include <QJsonObject>

#include "modbuscommon.h"

namespace xModbus {

ModbusDeviceEditor::ModbusDeviceEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeviceEditor)
{
    ui->setupUi(this);

    setupModebusDeviceType(ui->comboBoxDeviceType);
    setupRtuNames(ui->comboBoxRtuNames);
    setupRtuDataBits(ui->comboBoxRtuDataBits);
    setupRtuParity(ui->comboBoxRtuParity);
    setupRtuStopBits(ui->comboBoxRtuStopBits);
    setupRtuBaudRate(ui->comboBoxRtuBaudRate);

    connect(ui->pushButtonRefresh,
            &QPushButton::clicked,
            this,
            &ModbusDeviceEditor::onRefreshButtonClicked);
    connect(ui->comboBoxDeviceType,
            &QComboBox::currentIndexChanged,
            this,
            &ModbusDeviceEditor::onDeviceTypeChanged);

    onDeviceTypeChanged();
}

ModbusDeviceEditor::~ModbusDeviceEditor()
{
    delete ui;
}

void ModbusDeviceEditor::load(const QJsonObject &parameters)
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

QJsonObject ModbusDeviceEditor::save() const
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

void ModbusDeviceEditor::onRefreshButtonClicked()
{
    setupRtuNames(ui->comboBoxRtuNames);
}

void ModbusDeviceEditor::onDeviceTypeChanged()
{
    int currentType = ui->comboBoxDeviceType->currentData().toInt();
    bool isRtu = (currentType == static_cast<int>(XModbusType::RtuClient)
                  || currentType == static_cast<int>(XModbusType::RtuServer));
    bool isClient = (currentType == static_cast<int>(XModbusType::RtuClient)
                     || currentType == static_cast<int>(XModbusType::TcpClient));

    ui->lineSerialPort->setVisible(isRtu);
    ui->labelPortName->setVisible(isRtu);
    ui->labelBaudRate->setVisible(isRtu);
    ui->labelParity->setVisible(isRtu);
    ui->labelDataBits->setVisible(isRtu);
    ui->labelStopBits->setVisible(isRtu);
    ui->comboBoxRtuNames->setVisible(isRtu);
    ui->comboBoxRtuBaudRate->setVisible(isRtu);
    ui->comboBoxRtuParity->setVisible(isRtu);
    ui->comboBoxRtuDataBits->setVisible(isRtu);
    ui->comboBoxRtuStopBits->setVisible(isRtu);
    ui->pushButtonRefresh->setVisible(isRtu);

    ui->lineNetwork->setVisible(!isRtu);
    ui->labelIp->setVisible(!isRtu);
    ui->labelPort->setVisible(!isRtu);
    ui->comboBoxTcpAddress->setVisible(!isRtu);
    ui->spinBoxTcpPort->setVisible(!isRtu);

    ui->lineClient->setVisible(isClient);
    ui->labelRetries->setVisible(isClient);
    ui->labelTimeout->setVisible(isClient);
    ui->spinBoxNumberOfRetries->setVisible(isClient);
    ui->spinBoxTimeout->setVisible(isClient);

    ui->lineServer->setVisible(!isClient);
    ui->spinBoxServerAddress->setVisible(!isClient);
    ui->labelAddress->setVisible(!isClient);
    ui->checkBoxListenOnlyMode->setVisible(!isClient);

    adjustSize();
}

} // namespace xModbus