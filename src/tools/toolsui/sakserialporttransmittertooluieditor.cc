/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QJsonObject>

#include "sakserialporttransmittertool.h"
#include "SAKSerialPortTransmitterToolUiEditor.h"
#include "ui_SAKSerialPortTransmitterToolUiEditor.h"

SAKSerialPortTransmitterToolUiEditor::
    SAKSerialPortTransmitterToolUiEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKSerialPortTransmitterToolUiEditor)
{
    ui->setupUi(this);
    connect(ui->pushButtonApply, &QPushButton::clicked,
            this, &SAKSerialPortTransmitterToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &SAKSerialPortTransmitterToolUiEditor::reject);
}

SAKSerialPortTransmitterToolUiEditor::~SAKSerialPortTransmitterToolUiEditor()
{
    delete ui;
}

QJsonObject SAKSerialPortTransmitterToolUiEditor::parameters()
{
    SAKSerialPortTransmitterTool::ItemContextKey keys;
    QJsonObject obj;

    bool enable = ui->checkBoxEnable->isChecked();
    QString portName = ui->comboBoxPortName->currentText();
    int dataBits = ui->comboBoxDataBits->currentData().toInt();
    int stopBits = ui->comboBoxStopBits->currentData().toInt();
    int baudRate = ui->comboBoxBaudrate->currentData().toInt();
    int parity = ui->comboBoxParity->currentData().toInt();
    int flowControl = ui->comboBoxFlowControl->currentData().toInt();

    obj.insert(keys.enable, enable);
    obj.insert(keys.portName, portName);
    obj.insert(keys.dataBits, dataBits);
    obj.insert(keys.stopBits, stopBits);
    obj.insert(keys.parity, parity);
    obj.insert(keys.baudRate, baudRate);
    obj.insert(keys.flowControl, flowControl);

    return obj;

}

void SAKSerialPortTransmitterToolUiEditor::setParameters(const QJsonObject &params)
{
    SAKSerialPortTransmitterTool::ItemContextKey keys;
    bool enable = params.value(keys.enable).toBool();
    QString portName = params.value(keys.portName).toString();
    int dataBits = params.value(keys.dataBits).toInt();
    int stopBits = params.value(keys.stopBits).toInt();
    int parity = params.value(keys.parity).toInt();
    int baudRate = params.value(keys.baudRate).toInt();
    int flowControl = params.value(keys.flowControl).toInt();

    ui->checkBoxEnable->setChecked(enable);
    ui->comboBoxPortName->setCurrentText(portName);
    ui->comboBoxDataBits->setCurrentIndexFromData(dataBits);
    ui->comboBoxStopBits->setCurrentIndexFromData(stopBits);
    ui->comboBoxParity->setCurrentIndexFromData(parity);
    ui->comboBoxBaudrate->setCurrentIndexFromData(baudRate);
    ui->comboBoxFlowControl->setCurrentIndexFromData(flowControl);
}
