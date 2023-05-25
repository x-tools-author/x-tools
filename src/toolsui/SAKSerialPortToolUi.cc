/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSerialPortTool.hh"
#include "SAKSerialPortToolUi.hh"
#include "ui_SAKSerialPortToolUi.h"

SAKSerialPortToolUi::SAKSerialPortToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSerialPortToolUi)
{
    ui->setupUi(this);
}

SAKSerialPortToolUi::~SAKSerialPortToolUi()
{
    delete ui;
}

void SAKSerialPortToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                  const QString &settingsGroup)
{
    if (!(tool && tool->inherits("SAKSerialPortTool"))) {
        qCWarning(mLoggingCategory) << "Invalid type of communication tool!";
        return;
    }

    mTool = qobject_cast<SAKSerialPortTool*>(tool);
    mTool->setPortName(ui->comboBoxPortNames->currentText());
    mTool->setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
    mTool->setDataBits(ui->comboBoxDataBits->currentData().toInt());
    mTool->setStopBits(ui->comboBoxStopBits->currentData().toInt());
    mTool->setParity(ui->comboBoxParity->currentData().toInt());
    mTool->setFlowControl(ui->comboBoxFlowControl->currentData().toInt());

    connect(ui->comboBoxPortNames, &QComboBox::currentTextChanged,
            this, &SAKSerialPortToolUi::onComboBoxPortNamesCurrentTextChanged);
    connect(ui->comboBoxBaudRate, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortToolUi::onComboBoxBaudRateCurrentIndexChanged);
    connect(ui->comboBoxDataBits, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortToolUi::onComboBoxDataBitsCurrentIndexChanged);
    connect(ui->comboBoxStopBits, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortToolUi::onComboBoxStopBitsCurrentIndexChanged);
    connect(ui->comboBoxParity, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortToolUi::onComboBoxParityCurrentIndexChanged);
    connect(ui->comboBoxFlowControl, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortToolUi::onComboBoxFlowControlCurrentIndexChanged);

    ui->comboBoxPortNames->setGroupKey(settingsGroup, "portName");
    ui->comboBoxBaudRate->setGroupKey(settingsGroup, "baudRate", false);
    ui->comboBoxDataBits->setGroupKey(settingsGroup, "dataBits");
    ui->comboBoxStopBits->setGroupKey(settingsGroup, "stopBits");
    ui->comboBoxParity->setGroupKey(settingsGroup, "parity");
    ui->comboBoxFlowControl->setGroupKey(settingsGroup, "flowControl");
}

void SAKSerialPortToolUi::checkInitializingStatus()
{
    Q_ASSERT_X(mTool, __FUNCTION__, "Please call initialze() first!");
}

void SAKSerialPortToolUi::onComboBoxPortNamesCurrentTextChanged()
{
    checkInitializingStatus();
    mTool->setPortName(ui->comboBoxPortNames->currentText());
}

void SAKSerialPortToolUi::onComboBoxBaudRateCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
}

void SAKSerialPortToolUi::onComboBoxDataBitsCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setDataBits(ui->comboBoxDataBits->currentData().toInt());
}

void SAKSerialPortToolUi::onComboBoxStopBitsCurrentIndexChanged()
{
    mTool->setStopBits(ui->comboBoxStopBits->currentData().toInt());
}

void SAKSerialPortToolUi::onComboBoxParityCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setParity(ui->comboBoxParity->currentData().toInt());
}

void SAKSerialPortToolUi::onComboBoxFlowControlCurrentIndexChanged()
{
    checkInitializingStatus();
    mTool->setFlowControl(ui->comboBoxFlowControl->currentData().toInt());
}
