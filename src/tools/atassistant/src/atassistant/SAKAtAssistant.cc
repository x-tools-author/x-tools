/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDebug>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "SAKAtAssistant.hh"
#include "ui_SAKAtAssistant.h"

SAKAtAssistant::SAKAtAssistant(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::SAKAtAssistant)
    , serialPort_(Q_NULLPTR)
{
    ui_->setupUi(this);
    QSettings settings("settings.ini", QSettings::IniFormat);

    initUiCtx();
    initSettings();
    initSignals();
}

SAKAtAssistant::~SAKAtAssistant()
{
    delete ui_;
    if (serialPort_) {
        serialPort_->close();
        serialPort_->deleteLater();
        serialPort_ = Q_NULLPTR;
    }
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
    setupComboBoxText(uiCtx_.device.portName,
                       settingsCtx_.device.portName);
    setupComboBoxText(uiCtx_.device.baudRate,
                       settingsCtx_.device.baudRate);
    setupComboBoxIndex(uiCtx_.device.parity,
                       settingsCtx_.device.parity);
    setupComboBoxText(uiCtx_.device.dataBits,
                       settingsCtx_.device.dataBits);
    setupComboBoxText(uiCtx_.device.stopBits,
                       settingsCtx_.device.stopBits);
    setupComboBoxIndex(uiCtx_.device.flowControl,
                       settingsCtx_.device.flowControl);
}

void SAKAtAssistant::initSettingsInput()
{
    setupComboBoxText(uiCtx_.input.cycleSending,
                       settingsCtx_.input.cycleSending);
    setupComboBoxText(uiCtx_.input.textFormat,
                       settingsCtx_.input.textFormat);
    setupComboBoxText(uiCtx_.input.suffix,
                       settingsCtx_.input.suffix);
    setupCheckBox(uiCtx_.input.customSuffix,
                  settingsCtx_.input.customSuffix);
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
    connect(uiCtx_.device.portName,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onDevicePortNameChanged);
    connect(uiCtx_.device.baudRate,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onDeviceBaudRateChanged);
    connect(uiCtx_.device.parity,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onDeviceParityChanged);
    connect(uiCtx_.device.dataBits,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onDeviceDataBitsChanged);
    connect(uiCtx_.device.stopBits,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onDeviceStopBitsChanged);
    connect(uiCtx_.device.flowControl,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onDeviceFlowControlChanged);

    connect(uiCtx_.device.refresh, &QPushButton::clicked,
            this, &SAKAtAssistant::onDeviceRefreshClicked);
    connect(uiCtx_.device.open, &QPushButton::clicked,
            this, &SAKAtAssistant::onDeviceOpenClicked);
}

void SAKAtAssistant::initSignalsInput()
{
    connect(uiCtx_.input.textFormat,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onInputTextFormatChanged);
    connect(uiCtx_.input.suffix,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onInputSuffixChanged);
    connect(uiCtx_.input.customSuffix, &QCheckBox::clicked,
            this, &SAKAtAssistant::onInputCustomSuffixChanged);
    connect(uiCtx_.input.customSuffixLineEdit, &QLineEdit::textEdited,
            this, &SAKAtAssistant::onInputCustomSuffixLineEditChanged);
    connect(uiCtx_.input.atInput,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onInputCustomSuffixLineEditChanged);
}

void SAKAtAssistant::initSignalsOutput()
{
    connect(uiCtx_.output.textFormat,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onOutputTextFormatChanged);
    connect(uiCtx_.output.date, &QCheckBox::clicked,
            this, &SAKAtAssistant::onOutputDateChanged);
    connect(uiCtx_.output.time, &QCheckBox::clicked,
            this, &SAKAtAssistant::onOutputTimeChanged);
    connect(uiCtx_.output.ms, &QCheckBox::clicked,
            this, &SAKAtAssistant::onOutputMsChanged);
    connect(uiCtx_.output.rx, &QCheckBox::clicked,
            this, &SAKAtAssistant::onOutputRxChanged);
    connect(uiCtx_.output.tx, &QCheckBox::clicked,
            this, &SAKAtAssistant::onOutputTxChanged);
    connect(uiCtx_.output.clear, &QPushButton::clicked,
            this, &SAKAtAssistant::onOutputClearClicked);
}

void SAKAtAssistant::initSignalsCommand()
{
    connect(uiCtx_.command.interval,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onCommandIntervalChanged);
    connect(uiCtx_.command.timeout,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onCommandTimeoutChanged);
    connect(uiCtx_.command.number,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKAtAssistant::onCommandNumberChanged);
    connect(uiCtx_.command.startAutoTest, &QPushButton::clicked,
            this, &SAKAtAssistant::onCommandStartAutoTestClicked);
    connect(uiCtx_.command.importButton, &QPushButton::clicked,
            this, &SAKAtAssistant::onCommandImportButtonClicked);
    connect(uiCtx_.command.exportButton, &QPushButton::clicked,
            this, &SAKAtAssistant::onCommandExportButtonClicked);
}

void SAKAtAssistant::onDevicePortNameChanged()
{
    const QString portName = uiCtx_.device.portName->currentText();
    settings_.setValue(settingsCtx_.device.portName, portName);
}

void SAKAtAssistant::onDeviceBaudRateChanged()
{
    const QString bd = uiCtx_.device.baudRate->currentText();
    settings_.setValue(settingsCtx_.device.baudRate, bd);
}

void SAKAtAssistant::onDeviceParityChanged()
{
    const QString parity = uiCtx_.device.parity->currentText();
    settings_.setValue(settingsCtx_.device.parity, parity);
}

void SAKAtAssistant::onDeviceDataBitsChanged()
{
    const QString parity = uiCtx_.device.parity->currentText();
    settings_.setValue(settingsCtx_.device.parity, parity);
}

void SAKAtAssistant::onDeviceStopBitsChanged()
{
    const QString stopBit = uiCtx_.device.stopBits->currentText();
    settings_.setValue(settingsCtx_.device.stopBits, stopBit);
}

void SAKAtAssistant::onDeviceFlowControlChanged()
{
    const QString flowControl = uiCtx_.device.stopBits->currentText();
    settings_.setValue(settingsCtx_.device.stopBits, flowControl);
}

void SAKAtAssistant::onDeviceRefreshClicked()
{
    setupPortName(uiCtx_.device.portName);
}

void SAKAtAssistant::onDeviceOpenClicked()
{
    if (serialPort_) {
        serialPort_->close();
        serialPort_->deleteLater();
        serialPort_ = Q_NULLPTR;
        setupUiState(false);
    } else {
        QString portName = uiCtx_.device.portName->currentText();
        int baudRate = uiCtx_.device.baudRate->currentData().toInt();
        int stopBits = uiCtx_.device.stopBits->currentData().toInt();
        int parity = uiCtx_.device.parity->currentData().toInt();
        int dataBits = uiCtx_.device.dataBits->currentData().toInt();
        int flowControl = uiCtx_.device.flowControl->currentData().toInt();

        serialPort_ = new QSerialPort(this);
        serialPort_->setPortName(portName);
        serialPort_->setBaudRate(baudRate);
        serialPort_->setStopBits(QSerialPort::StopBits(stopBits));
        serialPort_->setParity(QSerialPort::Parity(parity));
        serialPort_->setDataBits(QSerialPort::DataBits(dataBits));
        serialPort_->setFlowControl(QSerialPort::FlowControl(flowControl));
        qInfo() << "PortName: " << serialPort_->portName()
                << "BudRate: " << serialPort_->baudRate()
                << "StopBits: " << serialPort_->stopBits()
                << "Parity: " << serialPort_->parity()
                << "DataBits: " << serialPort_->dataBits();

        if (serialPort_->open(QSerialPort::ReadWrite)) {
            setupUiState(true);
        } else {
            setUpdatesEnabled(false);
            qDebug() << tr("Can not open serial port: ")
                     << serialPort_->errorString();

        }
    }
}

void SAKAtAssistant::onInputCycleSendingChanged()
{
    int index = uiCtx_.input.cycleSending->currentIndex();
    settings_.setValue(settingsCtx_.input.cycleSending, index);
}

void SAKAtAssistant::onInputTextFormatChanged()
{
    int index = uiCtx_.input.textFormat->currentIndex();
    settings_.setValue(settingsCtx_.input.textFormat, index);
}

void SAKAtAssistant::onInputSuffixChanged()
{
    int index = uiCtx_.input.suffix->currentIndex();
    settings_.setValue(settingsCtx_.input.suffix, index);
}

void SAKAtAssistant::onInputCustomSuffixChanged()
{
    bool checked = uiCtx_.input.customSuffix->isChecked();
    settings_.setValue(settingsCtx_.input.customSuffix, checked);
}

void SAKAtAssistant::onInputCustomSuffixLineEditChanged()
{
    QString text = uiCtx_.input.customSuffixLineEdit->text();
    settings_.setValue(settingsCtx_.input.customSuffixLineEdit, text);
}

void SAKAtAssistant::onInputCustomAtInputChanged()
{

}

void SAKAtAssistant::onInputSendingClicked()
{
    if (serialPort_) {
        int timeout = uiCtx_.command.timeout->currentData().toInt();
        timeout = timeout < 100 ? 100 : timeout;
        QString command = uiCtx_.input.atInput->currentText();
        QByteArray suffix = uiCtx_.input.suffix->currentData().toByteArray();
        if (-1 == serialPort_->write(command.toLatin1() + suffix)) {
            qDebug() << tr("Can not send command to device:")
                        .arg(serialPort_->errorString());
        } else {
            serialPort_->waitForBytesWritten(timeout);
            QByteArray bytes = serialPort_->readAll();
            Q_UNUSED(bytes);
        }
    } else {
        setupDeviceNotOpenedMessageBox();
    }
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
    uiCtx_.output.textBrowser->clear();
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
    if (cb) {
        cb->clear();
        auto infos = QSerialPortInfo::availablePorts();
        for (QSerialPortInfo &info : infos) {
            cb->addItem(info.portName());
        }
    }
}

void SAKAtAssistant::setupBaudRate(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        QList<qint32> bds = QSerialPortInfo::standardBaudRates();
        for (qint32 &bd : bds) {
            cb->addItem(QString::number(bd), bd);
            if (bd == 9600) {
                cb->setCurrentText(QString::number(bd));
            }
        }
    }
}

void SAKAtAssistant::setupParity(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        cb->addItem(tr("NoParity"), QSerialPort::NoParity);
        cb->addItem(tr("OddParity"), QSerialPort::OddParity);
        cb->addItem(tr("EvenParity"), QSerialPort::EvenParity);
        cb->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
        cb->addItem(tr("MarkParity"), QSerialPort::MarkParity);
    }
}

void SAKAtAssistant::setupDataBits(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        cb->addItem("8", QSerialPort::Data8);
        cb->addItem("7", QSerialPort::Data7);
        cb->addItem("6", QSerialPort::Data6);
        cb->addItem("5", QSerialPort::Data5);
    }
}

void SAKAtAssistant::setupStopBits(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        cb->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
        cb->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
        cb->addItem("2", QSerialPort::TwoStop);
    }
}

void SAKAtAssistant::setupFlowControl(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        cb->addItem(tr("NoFlowControl"), QSerialPort::NoFlowControl);
        cb->addItem(tr("SoftwareFlowControl"), QSerialPort::SoftwareControl);
        cb->addItem(tr("HardwareFlowControl"), QSerialPort::HardwareControl);
    }
}

void SAKAtAssistant::setupTextFormat(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        cb->addItem("Bin", int(SAKEnumTextFormat::Bin));
        cb->addItem("Otc", int(SAKEnumTextFormat::Otc));
        cb->addItem("Dec", int(SAKEnumTextFormat::Dec));
        cb->addItem("Hex", int(SAKEnumTextFormat::Hex));
        cb->addItem("ASCII", int(SAKEnumTextFormat::ASCII));
        cb->addItem("UTF8", int(SAKEnumTextFormat::UTF8));
        cb->addItem("SYSTEM", int(SAKEnumTextFormat::SYSTEM));
    }
}

void SAKAtAssistant::setupCycleSending(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        cb->addItem(tr("Disable"), -1);
        for (int  i = 50; i <= 100; i += 10) {
            cb->addItem(QString::number(i), i);
        }

        for (int i = 110; i <= 200; i += 20) {
            cb->addItem(QString::number(i), i);
        }
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
    if (cb) {
        cb->clear();
        for (const QString &command : commands) {
            cb->addItem(command);
        }
    }
}

void SAKAtAssistant::setupCommandInterval(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        for (int i = 100; i <= 1000; i += 100) {
            cb->addItem(QString::number(i), i);
        }
    }
}

void SAKAtAssistant::setupCommandTimeout(QComboBox *cb)
{
    if (cb) {
        cb->clear();
        for (int i = 100; i <= 1000; i += 100) {
            cb->addItem(QString::number(i), i);
        }
    }
}

void SAKAtAssistant::setupCommandCycleNumber(QComboBox *cb)
{
    if (cb) {
        QString value = settings_.value(settingsCtx_.device.portName).toString();
        int index = cb->findText(value);
        if ((index >= 0) && (index < cb->count())) {
            cb->setCurrentIndex(index);
        }
    }
}

void SAKAtAssistant::setupComboBoxText(QComboBox *cb, const QString &key)
{
    if (cb) {
        QString value = settings_.value(key).toString();
        int index = cb->findText(value);
        if ((index >= 0) && (index < cb->count())) {
            cb->setCurrentIndex(index);
        }
    }
}

void SAKAtAssistant::setupComboBoxIndex(QComboBox *cb, const QString &key)
{
    if (cb) {
        int index = settings_.value(key).toInt();
        if ((index >= 0) && (index < cb->count())) {
            cb->setCurrentIndex(index);
        }
    }
}

void SAKAtAssistant::setupCheckBox(QCheckBox *cb, const QString &key)
{
    if (cb) {
        bool checked = settings_.value(key).toBool();
        cb->setChecked(checked);
    }
}

void SAKAtAssistant::setupLineEdit(QLineEdit *le, const QString &key)
{
    if (le) {
        QString text = settings_.value(key).toString();
        le->setText(text);
    }
}

void SAKAtAssistant::setupUiState(bool opened)
{
    uiCtx_.device.open->setText(opened ? tr("关闭") : tr("打开"));
    uiCtx_.device.refresh->setEnabled(!opened);
    uiCtx_.device.portName->setEnabled(opened);
    uiCtx_.device.parity->setEnabled(opened);
    uiCtx_.device.dataBits->setEnabled(opened);
    uiCtx_.device.stopBits->setEnabled(opened);
    uiCtx_.device.flowControl->setEnabled(opened);
}

void SAKAtAssistant::setupMessage(QTextBrowser *tb, const QString &message)
{
    if (tb) {
        tb->append(message);
    }
}

void SAKAtAssistant::setupDeviceNotOpenedMessageBox()
{
    QMessageBox::warning(
                this, tr("Device is Not Ready"),
                tr("The device is not opened, "
                   "please open the device and try again!"));
}
