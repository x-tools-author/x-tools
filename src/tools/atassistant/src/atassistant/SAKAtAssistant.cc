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
    uiCtx_.device.stopBits = ui_->stopBitsComboBox;
    uiCtx_.device.flowControl = ui_->flowControlComboBox;
    uiCtx_.device.refresh = ui_->refreshPushButton;
    uiCtx_.device.open = ui_->openPushButton;

    setupPortName(uiCtx_.device.portName);
    setupBaudRate(uiCtx_.device.baudRate);
    setupParity(uiCtx_.device.parity);
    setupFlowControl(uiCtx_.device.flowControl);
    setupDataBits(uiCtx_.device.dataBits);
    setupStopBits(uiCtx_.device.stopBits);
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

    setupTextFormat(uiCtx_.input.textFormat);
    setupCycleSending(uiCtx_.input.cycleSending);
    setupSuffix(uiCtx_.input.suffix);
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

    setupTextFormat(uiCtx_.output.textFormat);
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

    setupCommandCycleNumber(uiCtx_.command.number);
    setupCommandInterval(uiCtx_.command.interval);
    setupCommandTimeout(uiCtx_.command.timeout);
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
    cb->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        cb->addItem(QString::number(bd));
        if (bd == 9600) {
            cb->setCurrentText(QString::number(bd));
        }
    }
}

void SAKAtAssistant::setupParity(QComboBox *cb)
{
    cb->clear();
    cb->addItem(tr("NoParity"), QSerialPort::NoParity);
    cb->addItem(tr("OddParity"), QSerialPort::OddParity);
    cb->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    cb->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    cb->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void SAKAtAssistant::setupDataBits(QComboBox *cb)
{
    cb->clear();
    cb->addItem("8", QSerialPort::Data8);
    cb->addItem("7", QSerialPort::Data7);
    cb->addItem("6", QSerialPort::Data6);
    cb->addItem("5", QSerialPort::Data5);
}

void SAKAtAssistant::setupStopBits(QComboBox *cb)
{
    cb->clear();
    cb->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    cb->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    cb->addItem("2", QSerialPort::TwoStop);
}

void SAKAtAssistant::setupFlowControl(QComboBox *cb)
{
    cb->clear();
    cb->addItem(tr("NoFlowControl"), QSerialPort::NoFlowControl);
    cb->addItem(tr("SoftwareFlowControl"), QSerialPort::SoftwareControl);
    cb->addItem(tr("HardwareFlowControl"), QSerialPort::HardwareControl);
}

void SAKAtAssistant::setupTextFormat(QComboBox *cb)
{
    cb->clear();
    cb->addItem("Bin", int(SAKEnumTextFormat::Bin));
    cb->addItem("Otc", int(SAKEnumTextFormat::Otc));
    cb->addItem("Dec", int(SAKEnumTextFormat::Dec));
    cb->addItem("Hex", int(SAKEnumTextFormat::Hex));
    cb->addItem("ASCII", int(SAKEnumTextFormat::ASCII));
    cb->addItem("UTF8", int(SAKEnumTextFormat::UTF8));
    cb->addItem("SYSTEM", int(SAKEnumTextFormat::SYSTEM));
}

void SAKAtAssistant::setupCycleSending(QComboBox *cb)
{
    cb->clear();
    cb->addItem(tr("Disable"), -1);
    for (int  i = 50; i <= 100; i += 10) {
        cb->addItem(QString::number(i), i);
    }

    for (int i = 110; i <= 200; i += 20) {
        cb->addItem(QString::number(i), i);
    }
}

void SAKAtAssistant::setupSuffix(QComboBox *cb)
{
    cb->clear();
    cb->addItem(tr("None"), QByteArray(""));
    cb->addItem("\\r\\n", QByteArray("\r\n"));
    cb->addItem("\\n\\r", QByteArray("\n\r"));
    cb->addItem("\\r", QByteArray("\r"));
    cb->addItem("\\n", QByteArray("\n"));
}

void SAKAtAssistant::setupAtInput(QComboBox *cb, const QStringList &commands)
{
    cb->clear();
    for (const QString &command : commands) {
        cb->addItem(command);
    }
}

void SAKAtAssistant::setupCommandInterval(QComboBox *cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    for (int i = 100; i <= 1000; i += 100) {
        cb->addItem(QString::number(i), i);
    }
}

void SAKAtAssistant::setupCommandTimeout(QComboBox *cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    for (int i = 100; i <= 1000; i += 100) {
        cb->addItem(QString::number(i), i);
    }
}

void SAKAtAssistant::setupCommandCycleNumber(QComboBox *cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    for (int i = 1; i <= 5; i++) {
        cb->addItem(QString::number(i), i);
    }
}
