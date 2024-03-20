/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSerialPortToolUi.h"
#include "ui_xToolsSerialPortToolUi.h"

#include "xToolsSerialPortTool.h"

xToolsSerialPortToolUi::xToolsSerialPortToolUi(QWidget *parent)
    : xToolsCommunicationToolUi{parent}
    , ui(new Ui::xToolsSerialPortToolUi)
{
    ui->setupUi(this);
}

xToolsSerialPortToolUi::~xToolsSerialPortToolUi()
{
    delete ui;
}

void xToolsSerialPortToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    if (!(tool && tool->inherits("SAKSerialPortTool"))) {
        qWarning() << "Invalid type of communication tool!";
        return;
    }

    mTool = qobject_cast<xToolsSerialPortTool *>(tool);
    mTool->setPortName(ui->comboBoxPortNames->currentText());
    mTool->setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
    mTool->setDataBits(ui->comboBoxDataBits->currentData().toInt());
    mTool->setStopBits(ui->comboBoxStopBits->currentData().toInt());
    mTool->setParity(ui->comboBoxParity->currentData().toInt());
    mTool->setFlowControl(ui->comboBoxFlowControl->currentData().toInt());

    connect(ui->comboBoxPortNames,
            &QComboBox::currentTextChanged,
            this,
            &xToolsSerialPortToolUi::onComboBoxPortNamesCurrentTextChanged);
    connect(ui->comboBoxBaudRate,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSerialPortToolUi::onComboBoxBaudRateCurrentIndexChanged);
    connect(ui->comboBoxDataBits,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSerialPortToolUi::onComboBoxDataBitsCurrentIndexChanged);
    connect(ui->comboBoxStopBits,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSerialPortToolUi::onComboBoxStopBitsCurrentIndexChanged);
    connect(ui->comboBoxParity,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSerialPortToolUi::onComboBoxParityCurrentIndexChanged);
    connect(ui->comboBoxFlowControl,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSerialPortToolUi::onComboBoxFlowControlCurrentIndexChanged);

    ui->comboBoxPortNames->setGroupKey(settingsGroup, "portName");
    ui->comboBoxBaudRate->setGroupKey(settingsGroup, "baudRate", false);
    ui->comboBoxDataBits->setGroupKey(settingsGroup, "dataBits");
    ui->comboBoxStopBits->setGroupKey(settingsGroup, "stopBits");
    ui->comboBoxParity->setGroupKey(settingsGroup, "parity");
    ui->comboBoxFlowControl->setGroupKey(settingsGroup, "flowControl");
}

void xToolsSerialPortToolUi::checkInitializingStatus()
{
    Q_ASSERT_X(mTool, __FUNCTION__, "Please call initialze() first!");
}

void xToolsSerialPortToolUi::onComboBoxPortNamesCurrentTextChanged()
{
    checkInitializingStatus();
    mTool->setPortName(ui->comboBoxPortNames->currentText());
}

void xToolsSerialPortToolUi::onComboBoxBaudRateCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxDataBitsCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setDataBits(ui->comboBoxDataBits->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxStopBitsCurrentIndexChanged()
{
    mTool->setStopBits(ui->comboBoxStopBits->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxParityCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setParity(ui->comboBoxParity->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxFlowControlCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setFlowControl(ui->comboBoxFlowControl->currentData().toInt());
}
