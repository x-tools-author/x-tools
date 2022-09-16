/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QSerialPort>
#include <QSerialPortInfo>

#include "SAKAtAssistant.hh"
#include "ui_SAKAtAssistant.h"

SAKAtAssistant::SAKAtAssistant(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::SAKAtAssistant)
{
    ui_->setupUi(this);

    initUiCtx();
    initSettings();
    initSignals();
}

SAKAtAssistant::~SAKAtAssistant()
{
    delete ui_;
}

void SAKAtAssistant::initUiCtx()
{
    initUiCtxDevice();
    initUiCtxInput();
    initUiCtxOutput();
    initUiCtxCommand();
}

void SAKAtAssistant::initUiCtxDevice()
{
    uiCtx_.device.portName = ui_->portNameComboBox;
    uiCtx_.device.baudRate = ui_->baudRateComboBox;
    uiCtx_.device.parity = ui_->parityComboBox;
    uiCtx_.device.dataBits = ui_->dataBitsComboBox;
    uiCtx_.device.flowControl = ui_->flowControlComboBox;
    uiCtx_.device.refresh = ui_->refreshPushButton;
    uiCtx_.device.open = ui_->openPushButton;

    setupPortName(uiCtx_.device.portName);
}

void SAKAtAssistant::initUiCtxInput()
{
    uiCtx_.input.cycleSending = ui_->cucleSendingComboBox;
    uiCtx_.input.textFormat = ui_->inputTextFormatComboBox;
    uiCtx_.input.suffix = ui_->inputSuffixComboBox;
    uiCtx_.input.customSuffix = ui_->customSuffixCheckBox;
    uiCtx_.input.customSuffixLineEdit = ui_->customSuffixLineEdit;
    uiCtx_.input.atInput = ui_->atCommandComboBox;
    uiCtx_.input.sending = ui_->sendPushButton;
}

void SAKAtAssistant::initUiCtxOutput()
{
    uiCtx_.output.textFormat = ui_->outputTextFormatComboBox;
    uiCtx_.output.date = ui_->dateCheckBox;
    uiCtx_.output.time = ui_->timeCheckBox;
    uiCtx_.output.ms = ui_->msCheckBox;
    uiCtx_.output.rx = ui_->rxCheckBox;
    uiCtx_.output.tx = ui_->txCheckBox;
    uiCtx_.output.exportOutput = ui_->outputExportPushButton;
    uiCtx_.output.clear = ui_->outputClearPushButton;
    uiCtx_.output.textBrowser = ui_->textBrowser;
}

void SAKAtAssistant::initUiCtxCommand()
{
    uiCtx_.command.tableView = ui_->tableView;
    uiCtx_.command.interval = ui_->commandIntervalComboBox;
    uiCtx_.command.timeout = ui_->timeoutIntervalComboBox;
    uiCtx_.command.number = ui_->cycleTimesComboBox;
    uiCtx_.command.startAutoTest = ui_->startAutoTestPushButton;
    uiCtx_.command.importButton = ui_->importAtCommandsPushButton;
    uiCtx_.command.exportButton = ui_->exportAtCommandsPushButton;
}

void SAKAtAssistant::initSettings()
{
    initSettingsDevice();
    initSettingsInput();
    initSettingsOutput();
    initSettingsCommand();
}

void SAKAtAssistant::initSettingsDevice()
{

}

void SAKAtAssistant::initSettingsInput()
{

}

void SAKAtAssistant::initSettingsOutput()
{

}

void SAKAtAssistant::initSettingsCommand()
{

}

void SAKAtAssistant::initSignals()
{
    initSignalsDevice();
    initSignalsInput();
    initSignalsOutput();
    initSignalsCommand();
}

void SAKAtAssistant::initSignalsDevice()
{
    connect(uiCtx_.device.refresh, &QPushButton::clicked,
            this, &SAKAtAssistant::onDeviceRefreshClicked);
}

void SAKAtAssistant::initSignalsInput()
{

}

void SAKAtAssistant::initSignalsOutput()
{

}

void SAKAtAssistant::initSignalsCommand()
{

}

void SAKAtAssistant::onDevicePortNameChanged()
{

}

void SAKAtAssistant::onDeviceBaudRateChanged()
{

}

void SAKAtAssistant::onDeviceParityChanged()
{

}

void SAKAtAssistant::onDeviceDataBitsChanged()
{

}

void SAKAtAssistant::onDeviceRefreshClicked()
{
    setupPortName(uiCtx_.device.portName);
}

void SAKAtAssistant::onDeviceOpenClicked()
{

}

void SAKAtAssistant::onInputCycleSendingChanged()
{

}

void SAKAtAssistant::onInputTextFormatChanged()
{

}

void SAKAtAssistant::onInputSuffixChanged()
{

}

void SAKAtAssistant::onInputCustomSuffixChanged()
{

}

void SAKAtAssistant::onInputCustomSuffixLineEditChanged()
{

}

void SAKAtAssistant::onInputCustomAtInputChanged()
{

}

void SAKAtAssistant::onInputSendingClicked()
{

}

void SAKAtAssistant::onOutputTextFormatChanged()
{

}

void SAKAtAssistant::onOutputDateChanged()
{

}

void SAKAtAssistant::onOutputTimeChanged()
{

}

void SAKAtAssistant::onOutputMsChanged()
{

}

void SAKAtAssistant::onOutputRxChanged()
{

}

void SAKAtAssistant::onOutputTxChanged()
{

}

void SAKAtAssistant::onOutputExportOutputClikced()
{

}

void SAKAtAssistant::onOutputClearClicked()
{

}

void SAKAtAssistant::onCommandIntervalChanged()
{

}

void SAKAtAssistant::onCommandTimeoutChanged()
{

}

void SAKAtAssistant::onCommandNumberChanged()
{

}

void SAKAtAssistant::onCommandStartAutoTestClicked()
{

}

void SAKAtAssistant::onCommandImportButtonClicked()
{

}

void SAKAtAssistant::onCommandExportButtonClicked()
{

}

void SAKAtAssistant::setupPortName(QComboBox *cb)
{
    cb->clear();
    auto infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        cb->addItem(info.portName());
    }
}

void SAKAtAssistant::setupBaudRate(QComboBox *cb)
{

}

void SAKAtAssistant::setupParity(QComboBox *cb)
{

}

void SAKAtAssistant::setupDataBits(QComboBox *cb)
{

}

void SAKAtAssistant::setupFlowControl(QComboBox *cb)
{

}
