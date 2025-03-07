/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "page.h"
#include "ui_page.h"

#include <QMenu>
#include <QMessageBox>
#include <QWidgetAction>

#include "device/chartstest.h"
#include "device/chartstestui.h"
#include "device/device.h"
#include "device/deviceui.h"
#include "device/tcpclient.h"
#include "device/tcpclientui.h"
#include "device/tcpserver.h"
#include "device/tcpserverui.h"
#include "device/udpclient.h"
#include "device/udpclientui.h"
#include "device/udpserver.h"
#include "device/udpserverui.h"
#include "devicesettings.h"
#include "page/emitter/emitter.h"
#include "page/preset/preset.h"
#include "page/responder/responder.h"
#include "page/transfer/tcpclienttransfer.h"
#include "page/transfer/tcpclienttransferui.h"
#include "page/transfer/tcpservertransfer.h"
#include "page/transfer/tcpservertransferui.h"
#include "page/transfer/udpclienttransfer.h"
#include "page/transfer/udpclienttransferui.h"
#include "page/transfer/udpservertransfer.h"
#include "page/transfer/udpservertransferui.h"
#include "page/utilities/statistician.h"

#ifdef X_ENABLE_SERIAL_PORT
#include "device/serialport.h"
#include "device/serialportui.h"
#include "page/transfer/serialporttransfer.h"
#include "page/transfer/serialporttransferui.h"
#endif
#ifdef X_ENABLE_WEB_SOCKET
#include "device/websocketclient.h"
#include "device/websocketclientui.h"
#include "device/websocketserver.h"
#include "device/websocketserverui.h"
#include "page/transfer/websocketclienttransfer.h"
#include "page/transfer/websocketclienttransferui.h"
#include "page/transfer/websocketservertransfer.h"
#include "page/transfer/websocketservertransferui.h"
#endif
#ifdef X_ENABLE_CHARTS
#include "page/visualization/charts.h"
#include "page/visualization/chartsui.h"
#endif
#ifdef X_ENABLE_BLUETOOTH
#include "device/blecentral.h"
#include "device/blecentralui.h"
#endif

#include "common/crc.h"
#include "common/xtools.h"
#include "inputsettings.h"
#include "outputsettings.h"
#include "utilities/syntaxhighlighter.h"

struct ParameterKeys
{
    const QString showCharts{"showCharts"};
    const QString communicationType{"communicationType"};
    const QString communicationSettings{"communicationSettings"};
    const QString communication{"communication"};

    const QString outputFormat{"outputFormat"};
    const QString outputRx{"outputRx"};
    const QString outputTx{"outputTx"};
    const QString outputFlag{"outputFlag"};
    const QString outputDate{"outputDate"};
    const QString outputTime{"outputTime"};
    const QString outputMs{"outputMs"};
    const QString outputSettings{"outputSettings"};

    const QString cycleInterval{"cycleInterval"};
    const QString inputFormat{"inputFormat"};
    const QString inputSettings{"inputSettings"};

    const QString presetItems{"presetItems"};
    const QString emitterItems{"emitterItems"};
    const QString responserItems{"responserItems"};
    const QString serialPortTransferItems{"serialPortTransferItems"};
    const QString udpClientTransferItems{"udpClientTransferItems"};
    const QString udpServerTransferItems{"udpServerTransferItems"};
    const QString tcpClientTransferItems{"tcpClientTransferItems"};
    const QString tcpServerTransferItems{"tcpServerTransferItems"};
    const QString webSocketClientTransferItems{"webSocketClientTransferItems"};
    const QString webSocketServerTransferItems{"webSocketServerTransferItems"};

    const QString chartsItems{"chartsItems"};
} g_keys;

Page::Page(ControllerDirection direction, QSettings *settings, QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::Page}
    , m_io{nullptr}
    , m_ioUi{nullptr}
    , m_ioSettings{nullptr}
    , m_outputSettings{nullptr}
    , m_inputSettings{nullptr}
    , m_writeTimer{new QTimer(this)}
    , m_updateLabelInfoTimer{new QTimer(this)}
    , m_highlighter{new SyntaxHighlighter(this)}
    , m_rxStatistician{new Statistician(this)}
    , m_txStatistician{new Statistician(this)}
    , m_preset{new Preset(this)}
    , m_emitter{new Emitter(this)}
    , m_responder{new Responder(this)}
#ifdef X_ENABLE_SERIAL_PORT
    , m_serialPortTransfer(new SerialPortTransfer(this))
    , m_serialPortTransferUi(new SerialPortTransferUi(this))
#endif
    , m_udpClientTransfer(new UdpClientTransfer(this))
    , m_udpClientTransferUi(new UdpClientTransferUi())
    , m_udpServerTransfer(new UdpServerTransfer(this))
    , m_udpServerTransferUi(new UdpServerTransferUi())
    , m_tcpClientTransfer(new TcpClientTransfer(this))
    , m_tcpClientTransferUi(new TcpClientTransferUi())
    , m_tcpServerTransfer(new TcpServerTransfer(this))
    , m_tcpServerTransferUi(new TcpServerTransferUi())
#ifdef X_ENABLE_WEB_SOCKET
    , m_webSocketClientTransfer(new WebSocketClientTransfer(this))
    , m_webSocketClientTransferUi(new WebSocketClientTransferUi())
    , m_webSocketServerTransfer(new WebSocketServerTransfer(this))
    , m_webSocketServerTransferUi(new WebSocketServerTransferUi())
#endif
#ifdef X_ENABLE_CHARTS
    , m_charts{new Charts(this)}
    , m_chartsUi{new ChartsUi()}
#endif
    , m_settings{settings}
{
    if (settings == nullptr) {
        m_settings = new QSettings("IO_Page.ini", QSettings::IniFormat);
    }

    ui->setupUi(this);
    ui->widgetRxInfo->setupIO(m_rxStatistician);
    ui->widgetTxInfo->setupIO(m_txStatistician);
#ifdef X_ENABLE_CHARTS
    ui->widgetCharts->setLayout(new QHBoxLayout);
    ui->widgetCharts->layout()->addWidget(m_chartsUi);
    ui->widgetCharts->layout()->setContentsMargins(0, 0, 0, 0);
    ui->toolButtonCharts->setCheckable(true);
    ui->toolButtonCharts->setIcon(QIcon(":/res/icons/charts.svg"));
    ui->toolButtonCharts->setMenu(m_chartsUi->settingsMenu());
    ui->toolButtonCharts->setPopupMode(QToolButton::MenuButtonPopup);
    connect(ui->toolButtonCharts, &QToolButton::clicked, this, [this](bool checked) {
        ui->widgetCharts->setVisible(!ui->widgetCharts->isVisible());
    });
    connect(ui->lineEditInput, &QLineEdit::returnPressed, this, &Page::writeBytes);
#else
    ui->toolButtonCharts->setVisible(false);
#endif
    m_ioList << m_rxStatistician << m_txStatistician << m_preset << m_emitter << m_responder
             << m_udpClientTransfer << m_udpServerTransfer << m_tcpClientTransfer
             << m_tcpServerTransfer;
#ifdef X_ENABLE_SERIAL_PORT
    m_ioList << m_serialPortTransfer;
#endif
#ifdef X_ENABLE_WEB_SOCKET
    m_ioList << m_webSocketClientTransfer << m_webSocketServerTransfer;
#endif
#ifdef X_ENABLE_CHARTS
    m_ioList << m_charts;
#endif

    if (direction == ControllerDirection::Right) {
        auto *l = dynamic_cast<QHBoxLayout *>(layout());
        if (l) {
            auto item = l->takeAt(0);
            l->addItem(item);
        }
    }

    m_writeTimer->setInterval(1000);
    connect(m_writeTimer, &QTimer::timeout, this, &Page::writeBytes);

    m_updateLabelInfoTimer->setInterval(100);
    connect(m_updateLabelInfoTimer, &QTimer::timeout, this, &Page::updateLabelInfo);
    m_updateLabelInfoTimer->start();

    initUi();

    onShowStatisticianChanged(false);
    onDeviceTypeChanged();
}

Page::~Page()
{
    saveControllerParameters();
    delete ui;
}

QVariantMap Page::save()
{
    QVariantMap map;
    map.insert(g_keys.communicationType, ui->comboBoxDeviceTypes->currentData());
    map.insert(g_keys.communicationSettings, m_ioSettings->save());
    if (m_ioUi) {
        map.insert(g_keys.communication, m_ioUi->save());
    }

    map.insert(g_keys.showCharts, ui->toolButtonCharts->isChecked());

    map.insert(g_keys.outputFormat, ui->comboBoxOutputFormat->currentData());
    map.insert(g_keys.outputRx, ui->checkBoxOutputRx->isChecked());
    map.insert(g_keys.outputTx, ui->checkBoxOutputTx->isChecked());
    map.insert(g_keys.outputFlag, ui->checkBoxOutputFlag->isChecked());
    map.insert(g_keys.outputDate, ui->checkBoxOutputDate->isChecked());
    map.insert(g_keys.outputTime, ui->checkBoxOutputTime->isChecked());
    map.insert(g_keys.outputMs, ui->checkBoxOutputMs->isChecked());
    map.insert(g_keys.outputSettings, m_outputSettings->save());

    map.insert(g_keys.cycleInterval, ui->comboBoxInputInterval->currentData());
    map.insert(g_keys.inputFormat, ui->comboBoxInputFormat->currentData());
    map.insert(g_keys.inputSettings, m_inputSettings->save());

    map.insert(g_keys.presetItems, ui->tabPresets->save());
    map.insert(g_keys.emitterItems, ui->tabEmitter->save());
    map.insert(g_keys.responserItems, ui->tabResponder->save());
#ifdef X_ENABLE_SERIAL_PORT
    map.insert(g_keys.serialPortTransferItems, m_serialPortTransferUi->save());
#endif
    map.insert(g_keys.udpClientTransferItems, m_udpClientTransferUi->save());
    map.insert(g_keys.udpServerTransferItems, m_udpServerTransferUi->save());
    map.insert(g_keys.tcpClientTransferItems, m_tcpClientTransferUi->save());
    map.insert(g_keys.tcpServerTransferItems, m_tcpServerTransferUi->save());
#ifdef X_ENABLE_WEB_SOCKET
    map.insert(g_keys.webSocketClientTransferItems, m_webSocketClientTransferUi->save());
    map.insert(g_keys.webSocketServerTransferItems, m_webSocketServerTransferUi->save());
#endif
#ifdef X_ENABLE_CHARTS
    map.insert(g_keys.chartsItems, m_chartsUi->save());
#endif

    return map;
}

void Page::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    int communicationType = parameters.value(g_keys.communicationType).toInt();
    int index = ui->comboBoxDeviceTypes->findData(communicationType);
    QVariantMap communicationSettings = parameters.value(g_keys.communicationSettings).toMap();
    m_ioSettings->load(communicationSettings);
    ui->comboBoxDeviceTypes->setCurrentIndex(index == -1 ? 0 : index);
    if (m_ioUi) {
        m_ioUi->load(parameters.value(g_keys.communication).toMap());
    }

    bool showCharts = parameters.value(g_keys.showCharts).toBool();
    int outputFormat = parameters.value(g_keys.outputFormat).toInt();
    bool outputRx = parameters.value(g_keys.outputRx).toBool();
    bool outputTx = parameters.value(g_keys.outputTx).toBool();
    bool outputFlag = parameters.value(g_keys.outputFlag).toBool();
    bool outputDate = parameters.value(g_keys.outputDate).toBool();
    bool outputTime = parameters.value(g_keys.outputTime).toBool();
    bool outputMs = parameters.value(g_keys.outputMs).toBool();
    QVariantMap outputSettings = parameters.value(g_keys.outputSettings).toMap();

    ui->toolButtonCharts->setCheckable(showCharts);
    ui->widgetCharts->setVisible(showCharts);

    index = ui->comboBoxOutputFormat->findData(outputFormat);
    ui->comboBoxOutputFormat->setCurrentIndex(index == -1 ? 0 : index);
    ui->checkBoxOutputRx->setChecked(outputRx);
    ui->checkBoxOutputTx->setChecked(outputTx);
    ui->checkBoxOutputFlag->setChecked(outputFlag);
    ui->checkBoxOutputDate->setChecked(outputDate);
    ui->checkBoxOutputTime->setChecked(outputTime);
    ui->checkBoxOutputMs->setChecked(outputMs);
    m_outputSettings->load(outputSettings);

    int inputInterval = parameters.value(g_keys.cycleInterval).toInt();
    int inputFormat = parameters.value(g_keys.inputFormat).toInt();
    QVariantMap inputSettings = parameters.value(g_keys.inputSettings).toMap();

    index = ui->comboBoxInputInterval->findData(inputInterval);
    ui->comboBoxInputInterval->setCurrentIndex(index == -1 ? 0 : index);
    index = ui->comboBoxInputFormat->findData(inputFormat);
    ui->comboBoxInputFormat->setCurrentIndex(index == -1 ? 0 : index);
    m_inputSettings->load(inputSettings);

    // clang-format off
    ui->tabPresets->load(parameters.value(g_keys.presetItems).toMap());
    ui->tabEmitter->load(parameters.value(g_keys.emitterItems).toMap());
    ui->tabResponder->load(parameters.value(g_keys.responserItems).toMap());
#ifdef X_ENABLE_SERIAL_PORT
    m_serialPortTransferUi->load(parameters.value(g_keys.serialPortTransferItems).toMap());
#endif
    m_udpClientTransferUi->load(parameters.value(g_keys.udpClientTransferItems).toMap());
    m_udpServerTransferUi->load(parameters.value(g_keys.udpServerTransferItems).toMap());
    m_tcpClientTransferUi->load(parameters.value(g_keys.tcpClientTransferItems).toMap());
    m_tcpServerTransferUi->load(parameters.value(g_keys.tcpServerTransferItems).toMap());
#ifdef X_ENABLE_WEB_SOCKET
    m_webSocketClientTransferUi->load(parameters.value(g_keys.webSocketClientTransferItems).toMap());
    m_webSocketServerTransferUi->load(parameters.value(g_keys.webSocketServerTransferItems).toMap());
#endif
#ifdef X_ENABLE_CHARTS
    m_chartsUi->load(parameters.value(g_keys.chartsItems).toMap());
#endif
    // clang-format on

    onDeviceTypeChanged();
    onInputFormatChanged();
}

QTabWidget *Page::tabWidget()
{
    return ui->tabWidget;
}

QToolButton *Page::presetToolButton()
{
    return ui->toolButtonInputPreset;
}

void Page::inputBytes(const QByteArray &bytes)
{
    if (m_io && m_io->isWorking()) {
        m_io->inputBytes(bytes);
    }
}

void Page::prependOutputControl(QWidget *widget)
{
    ui->horizontalLayoutOutput->insertWidget(0, widget);
}

void Page::appendOutputControl(QWidget *widget)
{
    ui->horizontalLayoutOutput->addWidget(widget);
}

void Page::initUi()
{
#if 0
    const QIcon icon = QIcon(":/res/icons/settings.svg");
    ui->pushButtonCommunicationSettings->setIcon(icon);
    ui->pushButtonOutputSettings->setIcon(icon);
    ui->pushButtonInputSettings->setIcon(icon);
#endif
    ui->toolButtonInputPreset->setIcon(QIcon(":/res/icons/list.svg"));
    ui->toolButtonInputPreset->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->toolButtonInputPreset->setStyleSheet("QToolButton::menu-indicator{image: none;}");

    initUiDeviceControl();
    initUiOutputControl();
    initUiInputControl();
    initUiOutput();
    initUiInput();
}

void Page::initUiDeviceControl()
{
    connect(ui->comboBoxDeviceTypes,
            qOverload<int>(xComboBoxActivated),
            this,
            &Page::onDeviceTypeChanged);
    connect(ui->pushButtonCommunicationOpen,
            &QPushButton::clicked,
            this,
            &Page::onOpenButtonClicked);

    QPushButton *target = ui->pushButtonCommunicationSettings;
    m_ioSettings = new DeviceSettings();
    setupMenu(target, m_ioSettings);

    setupDeviceTypes(ui->comboBoxDeviceTypes);
}

void Page::initUiOutputControl()
{
    setupTextFormat(ui->comboBoxOutputFormat);
    ui->checkBoxOutputRx->setChecked(true);
    ui->checkBoxOutputTx->setChecked(true);
    ui->checkBoxOutputTime->setChecked(true);

    m_outputSettings = new OutputSettings();
    setupMenu(ui->pushButtonOutputSettings, m_outputSettings);

    connect(m_outputSettings,
            &OutputSettings::highlighterEnableChanged,
            this,
            &Page::onHighlighterEnableChanged);

    connect(m_outputSettings,
            &OutputSettings::highlighterKeywordsChanged,
            this,
            &Page::onHighlighterKeywordsChanged);
    connect(m_outputSettings,
            &OutputSettings::showStatisticianChanged,
            this,
            &Page::onShowStatisticianChanged);
    m_highlighter->setDocument(ui->textBrowserOutput->document());
}

void Page::initUiInputControl()
{
    connect(ui->comboBoxInputFormat,
            qOverload<int>(xComboBoxActivated),
            this,
            &Page::onInputFormatChanged);
    connect(ui->comboBoxInputInterval,
            qOverload<int>(xComboBoxActivated),
            this,
            &Page::onCycleIntervalChanged);
    connect(ui->pushButtonInputWriteBytes, &QPushButton::clicked, this, &Page::writeBytes);

    setupTextFormat(ui->comboBoxInputFormat);
    ui->comboBoxInputInterval->addItem(tr("Disable"), -1);
    for (int i = 10; i <= 50; i += 10) {
        ui->comboBoxInputInterval->addItem(QString::number(i), i);
    }
    for (int i = 100; i <= 1000; i += 100) {
        ui->comboBoxInputInterval->addItem(QString::number(i), i);
    }
    for (int i = 2000; i <= 10000; i += 1000) {
        ui->comboBoxInputInterval->addItem(QString::number(i), i);
    }

    m_inputSettings = new InputSettings();
    setupMenu(ui->pushButtonInputSettings, m_inputSettings);
}

void Page::initUiOutput()
{
#ifdef X_ENABLE_SERIAL_PORT
    ui->tabWidgetTransfers->addTab(m_serialPortTransferUi, tr("Serial Port"));
#endif
    ui->tabWidgetTransfers->addTab(m_udpClientTransferUi, tr("UDP Client"));
    ui->tabWidgetTransfers->addTab(m_udpServerTransferUi, tr("UDP Server"));
    ui->tabWidgetTransfers->addTab(m_tcpClientTransferUi, tr("TCP Client"));
    ui->tabWidgetTransfers->addTab(m_tcpServerTransferUi, tr("TCP Server"));
#ifdef X_ENABLE_WEB_SOCKET
    ui->tabWidgetTransfers->addTab(m_webSocketClientTransferUi, tr("WebSocket Client"));
    ui->tabWidgetTransfers->addTab(m_webSocketServerTransferUi, tr("WebSocket Server"));
#endif
    ui->tabPresets->setupIO(m_preset);
    ui->tabEmitter->setupIO(m_emitter);
    ui->tabResponder->setupIO(m_responder);
#ifdef X_ENABLE_SERIAL_PORT
    m_serialPortTransferUi->setupIO(m_serialPortTransfer);
#endif
    m_udpClientTransferUi->setupIO(m_udpClientTransfer);
    m_udpServerTransferUi->setupIO(m_udpServerTransfer);
    m_tcpClientTransferUi->setupIO(m_tcpClientTransfer);
    m_tcpServerTransferUi->setupIO(m_tcpServerTransfer);
#ifdef X_ENABLE_WEB_SOCKET
    m_webSocketClientTransferUi->setupIO(m_webSocketClientTransfer);
    m_webSocketServerTransferUi->setupIO(m_webSocketServerTransfer);
#endif
#ifdef X_ENABLE_CHARTS
    m_chartsUi->setupIO(m_charts);
#endif

    ui->toolButtonInputPreset->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonInputPreset->setMenu(ui->tabPresets->menu());
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidgetTransfers->setCurrentIndex(0);
}

void Page::initUiInput()
{
    // Nothing to do
}

void Page::onDeviceTypeChanged()
{
    if (m_ioUi != nullptr) {
        saveControllerParameters();

        m_ioUi->setParent(nullptr);
        m_ioUi->deleteLater();
        m_ioUi = nullptr;
    }

    if (!ui->comboBoxDeviceTypes->count()) {
        return;
    }

    int type = ui->comboBoxDeviceTypes->currentData().toInt();
    m_ioUi = newDeviceUi(type);
    if (m_ioUi) {
        loadControllerParameters();
        ui->verticalLayoutCommunicationController->addWidget(m_ioUi);

        QList<QWidget *> widgets = m_ioUi->communicationControllers();
        m_ioSettings->addWidgets(widgets);
    }
}

void Page::onCycleIntervalChanged()
{
    int interval = ui->comboBoxInputInterval->currentData().toInt();
    if (interval > 0) {
        m_writeTimer->start(interval);
    } else {
        m_writeTimer->stop();
    }
}

void Page::onInputFormatChanged()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    setupTextFormatValidator(ui->lineEditInput, format);
    ui->lineEditInput->clear();
    ui->lineEditInput->setPlaceholderText(bytes2string(QByteArray("(null)"), format));
}

void Page::onOpenButtonClicked()
{
    ui->pushButtonCommunicationOpen->setEnabled(false);
    if (m_io) {
        closeDevice();
    } else {
        openDevice();
    }
}

void Page::onHighlighterEnableChanged()
{
    bool enabled = m_outputSettings->isEnableHighlighter();
    m_highlighter->setEnabled(enabled);
}

void Page::onHighlighterKeywordsChanged()
{
    QStringList keywords = m_outputSettings->highlighterKeywords();
    m_highlighter->setKeywords(keywords);
}

void Page::onShowStatisticianChanged(bool checked)
{
    ui->labelRx->setVisible(checked);
    ui->labelTx->setVisible(checked);
    ui->lineRxTx->setVisible(checked);
    ui->widgetRxInfo->setVisible(checked);
    ui->widgetTxInfo->setVisible(checked);
}

void Page::onOpened()
{
    for (auto &io : m_ioList) {
        io->start();
    }

    setUiEnabled(false);
    ui->pushButtonCommunicationOpen->setEnabled(true);
    ui->pushButtonCommunicationOpen->setText(tr("Close"));
    onCycleIntervalChanged();
}

void Page::onClosed()
{
    for (auto &io : m_ioList) {
        io->exit();
        io->wait();
    }

    m_writeTimer->stop();
    setUiEnabled(true);
    ui->pushButtonCommunicationOpen->setEnabled(true);
    ui->pushButtonCommunicationOpen->setText(tr("Open"));
}

void Page::onErrorOccurred(const QString &error)
{
    closeDevice();
    if (!error.isEmpty()) {
        QMessageBox::warning(this, tr("Error Occurred"), error);
    }
}

void Page::onWarningOccurred(const QString &warning)
{
    QMessageBox::warning(this, tr("Warning"), warning);
}

void Page::onBytesRead(const QByteArray &bytes, const QString &from)
{
    m_ioSettings->saveData(bytes, false);
    m_rxStatistician->inputBytes(bytes);
    outputText(bytes, from, true);

    emit bytesRead(bytes, from);
}

void Page::onBytesWritten(const QByteArray &bytes, const QString &to)
{
    m_ioSettings->saveData(bytes, true);
    m_txStatistician->inputBytes(bytes);
    outputText(bytes, to, false);

    emit bytesWritten(bytes, to);
}

void Page::openDevice()
{
    int type = ui->comboBoxDeviceTypes->currentData().toInt();
    m_io = newDevice(type);
    if (m_io) {
        setUiEnabled(false);

        // clang-format off
        connect(m_io, &Device::opened, this, &Page::onOpened);
        connect(m_io, &Device::closed, this, &Page::onClosed);
        connect(m_io, &Device::bytesWritten, this, &Page::onBytesWritten);
        connect(m_io, &Device::bytesRead, this, &Page::onBytesRead);
        connect(m_io, &Device::errorOccurred, this, &Page::onErrorOccurred);
        connect(m_io, &Device::warningOccurred, this, &::Page::onWarningOccurred);
        connect(m_io, &Device::outputBytes, m_responder, &Responder::inputBytes);
#ifdef X_ENABLE_SERIAL_PORT
        connect(m_io, &Device::outputBytes, m_serialPortTransfer, &SerialPortTransfer::inputBytes);
#endif
        connect(m_io, &Device::outputBytes, m_udpClientTransfer, &UdpClientTransfer::inputBytes);
        connect(m_io, &Device::outputBytes, m_udpServerTransfer, &UdpServerTransfer::inputBytes);
        connect(m_io, &Device::outputBytes, m_tcpClientTransfer, &TcpClientTransfer::inputBytes);
        connect(m_io, &Device::outputBytes, m_tcpServerTransfer, &TcpServerTransfer::inputBytes);
#ifdef X_ENABLE_WEB_SOCKET
        connect(m_io, &Device::outputBytes, m_webSocketClientTransfer, &WebSocketClientTransfer::inputBytes);
        connect(m_io, &Device::outputBytes, m_webSocketServerTransfer, &WebSocketServerTransfer::inputBytes);
#endif
#ifdef X_ENABLE_CHARTS
        connect(m_io, &Device::outputBytes, m_charts, &Charts::inputBytes);
#endif
        connect(m_preset, &Preset::outputBytes, m_io, &Device::inputBytes);
        connect(m_emitter, &Preset::outputBytes, m_io, &Device::inputBytes);
        connect(m_responder, &Responder::outputBytes, m_io, &Device::inputBytes);
#ifdef X_ENABLE_SERIAL_PORT
        connect(m_serialPortTransfer, &SerialPortTransfer::outputBytes, m_io, &Device::inputBytes);
#endif
        connect(m_udpClientTransfer, &UdpClientTransfer::outputBytes, m_io, &Device::inputBytes);
        connect(m_udpServerTransfer, &UdpServerTransfer::outputBytes, m_io, &Device::inputBytes);
        connect(m_tcpClientTransfer, &TcpClientTransfer::outputBytes, m_io, &Device::inputBytes);
        connect(m_tcpServerTransfer, &TcpServerTransfer::outputBytes, m_io, &Device::inputBytes);
#ifdef X_ENABLE_WEB_SOCKET
        connect(m_webSocketClientTransfer, &WebSocketClientTransfer::outputBytes, m_io, &Device::inputBytes);
        connect(m_webSocketServerTransfer, &WebSocketServerTransfer::outputBytes, m_io, &Device::inputBytes);
#endif
        // clang-format on

#ifdef X_ENABLE_CHARTS
        m_charts->load(m_chartsUi->save());
#endif

        QVariantMap parameters = m_ioUi->save();
        m_ioUi->setupIO(m_io);
        m_io->load(parameters);
        m_io->openDevice();
    }
}

void Page::closeDevice()
{
    if (m_io) {
        m_io->exit();
        m_io->wait();
        m_io->deleteLater();
        m_io = nullptr;
    }

    if (m_ioUi) {
        m_ioUi->setupIO(nullptr);
    }
}

void Page::writeBytes()
{
    if (!m_io) {
        return;
    }

    auto parameters = m_inputSettings->parameters();
    QByteArray prefix = cookedAffixes(parameters.prefix);
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = cookedAffixes(parameters.suffix);

    QByteArray bytes;
    if (parameters.appendCrc) {
        bytes = prefix + payload + crc + suffix;
    } else {
        bytes = prefix + payload + suffix;
    }

    if (!bytes.isEmpty()) {
        m_io->inputBytes(bytes);
    }
}

void Page::updateLabelInfo()
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();

    QByteArray prefix = cookedAffixes(parameters.prefix);
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = cookedAffixes(parameters.suffix);

    QString prefixString = QString::fromLatin1(prefix.toHex()).toUpper();
    QString payloadString = QString::fromLatin1(payload.toHex()).toUpper();
    QString crcString = QString::fromLatin1(crc.toHex()).toUpper();
    QString suffixString = QString::fromLatin1(suffix.toHex()).toUpper();

    if (!parameters.bigEndian) {
        std::reverse(crc.begin(), crc.end());
    }
    QString const crcValue = QString("CRC: 0x%1").arg(QString::fromLatin1(crc.toHex()).toUpper());
    ui->labelCrc->setText(crcValue);

    if (!parameters.appendCrc) {
        crcString.clear();
    }
    QString info = tr("[%1][%2][%3][%4]").arg(prefixString, payloadString, crcString, suffixString);
    QFontMetrics fontWidth(ui->labelInfo->font());
    info = fontWidth.elidedText(info, Qt::ElideMiddle, ui->labelInfo->width());
    if (parameters.showDataPreview) {
        ui->labelInfo->setText(info);
    } else {
        ui->labelInfo->clear();
    }
}

void Page::setupMenu(QPushButton *target, QWidget *actionWidget)
{
    QMenu *menu = new QMenu(target);
    QWidgetAction *action = new QWidgetAction(menu);
    action->setDefaultWidget(actionWidget);
    menu->addAction(action);
    target->setMenu(menu);
}

void Page::setUiEnabled(bool enabled)
{
    if (m_ioUi) {
        m_ioUi->setUiEnabled(enabled);
    }

    ui->comboBoxDeviceTypes->setEnabled(enabled);
}

QString dateTimeString(bool showDate, bool showTime, bool showMs)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString str;
    if (showDate) {
        QString const dateString = dateTime.toString("yyyy-MM-dd");
        str += dateString;
        str += " ";
    }

    if (showTime) {
        QString const timeString = dateTime.toString("hh:mm:ss");
        str += timeString;
        str += " ";
    }

    if (showMs) {
        QString const msString = dateTime.toString("zzz");
        str = str.trimmed();
        if (!str.isEmpty()) {
            str += ".";
        }
        str += msString;
    }

    return str;
}

QString flagString(bool isRx, const QString &flag)
{
    QString str;
    if (flag.isEmpty()) {
        return isRx ? QString("Rx") + flag : QStringLiteral("Tx") + flag;
    }

    return str;
}

void Page::outputText(const QByteArray &bytes, const QString &flag, bool isRx)
{
    bool showRx = ui->checkBoxOutputRx->isChecked();
    bool showTx = ui->checkBoxOutputTx->isChecked();
    bool showFlag = ui->checkBoxOutputFlag->isChecked();
    bool showDate = ui->checkBoxOutputDate->isChecked();
    bool showTime = ui->checkBoxOutputTime->isChecked();
    bool showMs = ui->checkBoxOutputMs->isChecked();
    int format = ui->comboBoxOutputFormat->currentData().toInt();

    if (isRx && !showRx) {
        return;
    }

    if (!isRx && !showTx) {
        return;
    }

    QString dateTimeString = ::dateTimeString(showDate, showTime, showMs);
    QString text = bytes2string(bytes, format);
    QString rxTx = isRx ? QStringLiteral("Rx") : QStringLiteral("Tx");
    rxTx = QString("<font color=%1>%2</font>").arg(isRx ? "blue" : "green", rxTx);

    QString header;
    if (showFlag) {
        header = QString("%1 %2 %3").arg(rxTx, dateTimeString, flag);
    } else {
        header = QString("%1 %2").arg(rxTx, dateTimeString);
    }

    header = header.trimmed();
    header = QString("<font color=silver>[%1]</font>").arg(header);
    QString outputText = QString("%1 %2").arg(header, text);
    outputText = outputText.replace("\r", "\\r");
    outputText = outputText.replace("\n", "\\n");
    if (m_outputSettings->isEnableFilter()) {
        QString filter = m_outputSettings->filterText();
        if (outputText.contains(filter)) {
            ui->textBrowserOutput->append(outputText);
        }
    } else {
        ui->textBrowserOutput->append(outputText);
    }
}

void Page::saveControllerParameters()
{
    if (m_ioUi != nullptr) {
        auto parameters = m_ioUi->save();
        int type = static_cast<int>(m_ioUi->type());
        m_settings->setValue(QString("controller_%1").arg(type), parameters);
    }
}

void Page::loadControllerParameters()
{
    int type = ui->comboBoxDeviceTypes->currentData().toInt();
    auto parameters = m_settings->value(QString("controller_%1").arg(type));
    if (!parameters.isNull()) {
        m_ioUi->load(parameters.toMap());
    }
}

QByteArray Page::payload() const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    QString text = ui->lineEditInput->text();
    if (text.isEmpty()) {
        text = ui->lineEditInput->placeholderText();
    }

    int format = ui->comboBoxInputFormat->currentData().toInt();
    text = cookedEscapeCharacter(text, parameters.escapeCharacter);
    QByteArray payload = string2bytes(text, format);
    return payload;
}

QByteArray Page::crc(const QByteArray &payload) const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    CRC::Context ctx;
    ctx.algorithm = static_cast<CRC::Algorithm>(parameters.algorithm);
    ctx.startIndex = parameters.startIndex;
    ctx.endIndex = parameters.endIndex;
    ctx.bigEndian = parameters.bigEndian;
    ctx.data = payload;

    return CRC::calculate(ctx);
}

Device *Page::newDevice(int type)
{
    switch (type) {
#ifdef X_ENABLE_SERIAL_PORT
    case static_cast<int>(DeviceType::SerialPort):
        return new SerialPort(QCoreApplication::instance());
#endif
#ifdef X_ENABLE_BLUETOOTH
    case static_cast<int>(DeviceType::BleCentral):
        return new BleCentral(QCoreApplication::instance());
#endif
    case static_cast<int>(DeviceType::UdpClient):
        return new UdpClient(QCoreApplication::instance());
    case static_cast<int>(DeviceType::UdpServer):
        return new UdpServer(QCoreApplication::instance());
    case static_cast<int>(DeviceType::TcpClient):
        return new TcpClient(QCoreApplication::instance());
    case static_cast<int>(DeviceType::TcpServer):
        return new TcpServer(QCoreApplication::instance());
#ifdef X_ENABLE_WEB_SOCKET
    case static_cast<int>(DeviceType::WebSocketClient):
        return new WebSocketClient(QCoreApplication::instance());
    case static_cast<int>(DeviceType::WebSocketServer):
        return new WebSocketServer(QCoreApplication::instance());
#endif
    case static_cast<int>(DeviceType::ChartsTest):
        return new ChartsTest(QCoreApplication::instance());
    default:
        qWarning("Unknown device type:%d", type);
        return nullptr;
    }
}

DeviceUi *Page::newDeviceUi(int type)
{
    switch (type) {
#ifdef X_ENABLE_SERIAL_PORT
    case static_cast<int>(DeviceType::SerialPort):
        return new SerialPortUi();
#endif
#ifdef X_ENABLE_BLUETOOTH
    case static_cast<int>(DeviceType::BleCentral):
        return new BleCentralUi();
#endif
    case static_cast<int>(DeviceType::UdpClient):
        return new UdpClientUi();
    case static_cast<int>(DeviceType::UdpServer):
        return new UdpServerUi();
    case static_cast<int>(DeviceType::TcpClient):
        return new TcpClientUi();
    case static_cast<int>(DeviceType::TcpServer):
        return new TcpServerUi();
#ifdef X_ENABLE_WEB_SOCKET
    case static_cast<int>(DeviceType::WebSocketClient):
        return new WebSocketClientUi();
    case static_cast<int>(DeviceType::WebSocketServer):
        return new WebSocketServerUi();
    case static_cast<int>(DeviceType::ChartsTest):
        return new ChartsTestUi();
#endif
    default:
        return nullptr;
    }
}
