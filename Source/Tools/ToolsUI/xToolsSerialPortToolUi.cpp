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

#include <QDebug>

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

void xToolsSerialPortToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                     const QString &settingsGroup)
{
    if (!(tool && tool->inherits("xToolsSerialPortTool"))) {
        qWarning() << "Invalid type of communication tool!";
        return;
    }

    m_tool = qobject_cast<xToolsSerialPortTool *>(tool);
    m_tool->setPortName(ui->comboBoxPortNames->currentText());
    m_tool->setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
    m_tool->setDataBits(ui->comboBoxDataBits->currentData().toInt());
    m_tool->setStopBits(ui->comboBoxStopBits->currentData().toInt());
    m_tool->setParity(ui->comboBoxParity->currentData().toInt());
    m_tool->setFlowControl(ui->comboBoxFlowControl->currentData().toInt());

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
    Q_ASSERT_X(m_tool, __FUNCTION__, "Please call initialze() first!");
}

void xToolsSerialPortToolUi::onComboBoxPortNamesCurrentTextChanged()
{
    checkInitializingStatus();
    m_tool->setPortName(ui->comboBoxPortNames->currentText());
}

void xToolsSerialPortToolUi::onComboBoxBaudRateCurrentIndexChanged()
{
    checkInitializingStatus();
    m_tool->setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxDataBitsCurrentIndexChanged()
{
    checkInitializingStatus();
    m_tool->setDataBits(ui->comboBoxDataBits->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxStopBitsCurrentIndexChanged()
{
    m_tool->setStopBits(ui->comboBoxStopBits->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxParityCurrentIndexChanged()
{
    checkInitializingStatus();
    m_tool->setParity(ui->comboBoxParity->currentData().toInt());
}

void xToolsSerialPortToolUi::onComboBoxFlowControlCurrentIndexChanged()
{
    checkInitializingStatus();
    m_tool->setFlowControl(ui->comboBoxFlowControl->currentData().toInt());
}
