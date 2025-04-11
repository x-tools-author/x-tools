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
#include <QTimer>
#include <QWidgetAction>

#include "device/chartstestui.h"
#include "device/device.h"
#include "device/deviceui.h"
#include "device/localserverui.h"
#include "device/localsocketui.h"
#include "device/tcpclientui.h"
#include "device/tcpserverui.h"
#include "device/udpclientui.h"
#include "device/udpserverui.h"
#include "devicesettings.h"
#include "emitter/emitterview.h"
#include "page/preset/presetview.h"
#include "page/responder/responderview.h"

#ifdef X_ENABLE_SERIAL_PORT
#include "device/serialportui.h"
#endif
#ifdef X_ENABLE_WEB_SOCKET
#include "device/websocketclientui.h"
#include "device/websocketserverui.h"
#endif

#ifdef X_ENABLE_BLUETOOTH
#include "device/blecentralui.h"
#endif

#ifdef X_ENABLE_CHARTS
#include "page/charts/chartsview.h"
#endif

#include "common/crc.h"
#include "common/xtools.h"
#include "inputsettings.h"
#include "outputsettings.h"
#include "utilities/statistician.h"
#include "utilities/syntaxhighlighter.h"

struct ParameterKeys
{
    const QString showCharts{"showCharts"};
    const QString showSearch{"showSearch"};
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
    const QString outputWrap{"outputWrap"};
    const QString outputTerminalMode{"outputTerminalMode"};
    const QString outputSettings{"outputSettings"};

    const QString cycleInterval{"cycleInterval"};
    const QString inputFormat{"inputFormat"};
    const QString inputSettings{"inputSettings"};

    const QString presetItems{"presetItems"};
    const QString emitterItems{"emitterItems"};
    const QString responserItems{"responserItems"};
    const QString transfers{"transfers"};

    const QString chartsItems{"chartsItems"};
} g_keys;

Page::Page(ControllerDirection direction, QSettings *settings, QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::Page}
    , m_deviceController{nullptr}
    , m_ioSettings{nullptr}
    , m_outputSettings{nullptr}
    , m_inputSettings{nullptr}
    , m_writeTimer{new QTimer(this)}
    , m_updateLabelInfoTimer{new QTimer(this)}
    , m_highlighter{new SyntaxHighlighter(this)}
    , m_settings{settings}
{
    if (settings == nullptr) {
        m_settings = new QSettings("IO_Page.ini", QSettings::IniFormat);
    }

    ui->setupUi(this);
    m_rxStatistician = new Statistician(ui->labelRxInfo, this);
    m_txStatistician = new Statistician(ui->labelTxInfo, this);

#ifdef X_ENABLE_CHARTS
    m_chartsView = new ChartsView(this);
    ui->widgetCharts->setLayout(new QHBoxLayout);
    ui->widgetCharts->layout()->setContentsMargins(0, 0, 0, 0);
    ui->widgetCharts->layout()->addWidget(m_chartsView);

    ui->widgetChartsController->setLayout(new QHBoxLayout);
    ui->widgetChartsController->layout()->setContentsMargins(0, 0, 0, 0);
    for (auto &w : m_chartsView->chartControllers()) {
        ui->widgetChartsController->layout()->addWidget(w);
    }

    ui->toolButtonCharts->setCheckable(true);
    ui->toolButtonCharts->setIcon(QIcon(":/res/icons/charts.svg"));
    connect(ui->toolButtonCharts, &QToolButton::clicked, this, &Page::updateChartUi);
#else
    hideChartsWidgets();
#endif

    if (direction == ControllerDirection::Right) {
        auto *l = dynamic_cast<QHBoxLayout *>(layout());
        if (l) {
            auto item = l->takeAt(0);
            l->addItem(item);
        }
    }

    // Search tool button
    ui->widgetSearch->hide();
    ui->toolButtonSearch->setCheckable(true);
    ui->toolButtonSearch->setIcon(QIcon(":/res/icons/search.svg"));
    connect(ui->toolButtonSearch, &QToolButton::clicked, this, [this](bool checked) {
        ui->widgetSearch->setVisible(checked);
    });

    m_writeTimer->setInterval(1000);
    connect(m_writeTimer, &QTimer::timeout, this, &Page::writeBytes);

    m_updateLabelInfoTimer->setInterval(100);
    connect(m_updateLabelInfoTimer, &QTimer::timeout, this, &Page::updateLabelInfo);
    m_updateLabelInfoTimer->start();

    connect(ui->lineEditInput, &QLineEdit::returnPressed, this, &Page::writeBytes);
    connect(ui->checkBoxWrap, &QCheckBox::clicked, this, &Page::onWrapModeChanged);

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
    if (m_deviceController) {
        map.insert(g_keys.communication, m_deviceController->save());
    }

    map.insert(g_keys.showCharts, ui->toolButtonCharts->isChecked());
    map.insert(g_keys.showSearch, ui->toolButtonSearch->isChecked());

    map.insert(g_keys.outputFormat, ui->comboBoxOutputFormat->currentData());
    map.insert(g_keys.outputRx, ui->checkBoxOutputRx->isChecked());
    map.insert(g_keys.outputTx, ui->checkBoxOutputTx->isChecked());
    map.insert(g_keys.outputFlag, ui->checkBoxOutputFlag->isChecked());
    map.insert(g_keys.outputDate, ui->checkBoxOutputDate->isChecked());
    map.insert(g_keys.outputTime, ui->checkBoxOutputTime->isChecked());
    map.insert(g_keys.outputMs, ui->checkBoxOutputMs->isChecked());
    map.insert(g_keys.outputSettings, m_outputSettings->save());
    map.insert(g_keys.outputWrap, ui->checkBoxWrap->isChecked());
    map.insert(g_keys.outputTerminalMode, ui->checkBoxTerminalMode->isChecked());

    map.insert(g_keys.cycleInterval, ui->comboBoxInputInterval->currentData());
    map.insert(g_keys.inputFormat, ui->comboBoxInputFormat->currentData());
    map.insert(g_keys.inputSettings, m_inputSettings->save());

    map.insert(g_keys.presetItems, ui->tabPreset->save());
    map.insert(g_keys.emitterItems, ui->tabEmitter->save());
    map.insert(g_keys.responserItems, ui->tabResponder->save());

    if (ui->tabTransfers->isEnabled()) {
        map.insert(g_keys.transfers, ui->tabTransfers->save());
    }

#ifdef X_ENABLE_CHARTS
    map.insert(g_keys.chartsItems, m_chartsView->save());
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
    if (m_deviceController) {
        m_deviceController->load(parameters.value(g_keys.communication).toMap());
    }

    bool showCharts = parameters.value(g_keys.showCharts).toBool();
    bool showSearch = parameters.value(g_keys.showSearch).toBool();
    int outputFormat = parameters.value(g_keys.outputFormat).toInt();
    bool outputRx = parameters.value(g_keys.outputRx).toBool();
    bool outputTx = parameters.value(g_keys.outputTx).toBool();
    bool outputFlag = parameters.value(g_keys.outputFlag).toBool();
    bool outputDate = parameters.value(g_keys.outputDate).toBool();
    bool outputTime = parameters.value(g_keys.outputTime).toBool();
    bool outputMs = parameters.value(g_keys.outputMs).toBool();
    bool outputWrap = parameters.value(g_keys.outputWrap).toBool();
    bool outputTerminalMode = parameters.value(g_keys.outputTerminalMode).toBool();
    QVariantMap outputSettings = parameters.value(g_keys.outputSettings).toMap();

    ui->toolButtonCharts->setChecked(showCharts);
    ui->toolButtonSearch->setChecked(showSearch);
    ui->widgetSearch->setVisible(showSearch);
    index = ui->comboBoxOutputFormat->findData(outputFormat);
    ui->comboBoxOutputFormat->setCurrentIndex(index == -1 ? 0 : index);
    ui->checkBoxOutputRx->setChecked(outputRx);
    ui->checkBoxOutputTx->setChecked(outputTx);
    ui->checkBoxOutputFlag->setChecked(outputFlag);
    ui->checkBoxOutputDate->setChecked(outputDate);
    ui->checkBoxOutputTime->setChecked(outputTime);
    ui->checkBoxOutputMs->setChecked(outputMs);
    ui->checkBoxWrap->setChecked(outputWrap);
    ui->checkBoxTerminalMode->setChecked(outputTerminalMode);
    m_outputSettings->load(outputSettings);

    int inputInterval = parameters.value(g_keys.cycleInterval).toInt();
    int inputFormat = parameters.value(g_keys.inputFormat).toInt();
    QVariantMap inputSettings = parameters.value(g_keys.inputSettings).toMap();

    index = ui->comboBoxInputInterval->findData(inputInterval);
    ui->comboBoxInputInterval->setCurrentIndex(index == -1 ? 0 : index);
    index = ui->comboBoxInputFormat->findData(inputFormat);
    ui->comboBoxInputFormat->setCurrentIndex(index == -1 ? 0 : index);
    m_inputSettings->load(inputSettings);

#ifdef X_ENABLE_CHARTS
    m_chartsView->load(parameters.value(g_keys.chartsItems).toMap());
#endif

    ui->tabPreset->load(parameters.value(g_keys.presetItems).toMap());
    ui->tabEmitter->load(parameters.value(g_keys.emitterItems).toMap());
    ui->tabResponder->load(parameters.value(g_keys.responserItems).toMap());
    ui->tabTransfers->load(parameters.value(g_keys.transfers).toMap());

    onDeviceTypeChanged();
    onInputFormatChanged();
    onWrapModeChanged();
    updateChartUi();
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
    if (m_deviceController->device()) {
        m_deviceController->device()->writeBytes(bytes);
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

void Page::hideChartsWidgets()
{
    m_enableChars = false;
    ui->toolButtonCharts->setVisible(false);
    ui->widgetCharts->setVisible(false);
    ui->widgetChartsController->setVisible(false);
}

void Page::hideTransferWidgets()
{
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tabTransfers));
    ui->tabTransfers->setEnabled(false);
    ui->tabTransfers->hide();
}

void Page::removeTestDevices()
{
    for (int i = 0; i < ui->comboBoxDeviceTypes->count(); i++) {
        int type = ui->comboBoxDeviceTypes->itemData(i).toInt();
        if (type == static_cast<int>(DeviceType::ChartsTest)) {
            ui->comboBoxDeviceTypes->removeItem(i);
            break;
        }
    }
}

void Page::initUi()
{
#if 0
    const QIcon icon = QIcon(":/res/icons/settings.svg");
    ui->pushButtonDeviceSettings->setIcon(icon);
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
    connect(ui->pushButtonDeviceOpen, &QPushButton::clicked, this, &Page::onOpenButtonClicked);

    QPushButton *target = ui->pushButtonDeviceSettings;
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
    ui->toolButtonInputPreset->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonInputPreset->setMenu(ui->tabPreset->menu());
    ui->tabWidget->setCurrentIndex(0);
    ui->tabTransfers->setCurrentIndex(0);

    // 连接搜索相关的信号和槽
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Page::onSearchButtonClicked);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &Page::onSearchTextChanged);
    connect(ui->checkBoxRegex, &QCheckBox::clicked, this, [this](int) { performSearch(); });
    connect(ui->checkBoxMatchCase, &QCheckBox::clicked, this, [this](int) { performSearch(); });
    connect(ui->checkBoxWholeWord, &QCheckBox::clicked, this, [this](int) { performSearch(); });
}

void Page::initUiInput()
{
    // Nothing to do
}

void Page::onDeviceTypeChanged()
{
    if (m_deviceController != nullptr) {
        saveControllerParameters();

        m_deviceController->setParent(nullptr);
        m_deviceController->deleteLater();
        m_deviceController = nullptr;
    }

    if (!ui->comboBoxDeviceTypes->count()) {
        return;
    }

    int type = ui->comboBoxDeviceTypes->currentData().toInt();
    m_deviceController = newDeviceUi(type);
    if (!m_deviceController) {
        qWarning() << "Failed to create device controller";
        return;
    }

    setupDevice(m_deviceController->device());
    loadControllerParameters();
    ui->verticalLayoutDeviceController->addWidget(m_deviceController);

    QList<QWidget *> widgets = m_deviceController->deviceControllers();
    m_ioSettings->addWidgets(widgets);
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
    ui->pushButtonDeviceOpen->setEnabled(false);
    if (m_deviceController->device() && m_deviceController->device()->isRunning()) {
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
    ui->widgetRxTxInfo->setVisible(checked);
}

void Page::onOpened()
{
#ifdef X_ENABLE_CHARTS
    m_chartsView->resetCharts();
#endif

    setUiEnabled(false);
    onCycleIntervalChanged();

    ui->pushButtonDeviceOpen->setEnabled(true);
    ui->pushButtonDeviceOpen->setText(tr("Close"));
}

void Page::onClosed()
{
    m_writeTimer->stop();

    setUiEnabled(true);
    ui->pushButtonDeviceOpen->setEnabled(true);
    ui->pushButtonDeviceOpen->setText(tr("Open"));
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

#ifdef X_ENABLE_CHARTS
    if (m_enableChars) {
        m_chartsView->inputBytes(bytes);
    }
#endif

    ui->tabResponder->inputBytes(bytes);

    if (ui->tabTransfers->isEnabled()) {
        ui->tabTransfers->inputBytes(bytes);
    }

    emit bytesRead(bytes, from);
}

void Page::onBytesWritten(const QByteArray &bytes, const QString &to)
{
    m_ioSettings->saveData(bytes, true);
    m_txStatistician->inputBytes(bytes);
    outputText(bytes, to, false);

    emit bytesWritten(bytes, to);
}

void Page::onWrapModeChanged()
{
    if (ui->checkBoxWrap->isChecked()) {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::WrapMode::WordWrap);
    } else {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::WrapMode::NoWrap);
    }
}

void Page::openDevice()
{
    if (!m_deviceController->device()) {
        qWarning() << "Failed to create device";
        return;
    }

    setUiEnabled(false);

    m_rxStatistician->reset();
    m_txStatistician->reset();
    m_deviceController->openDevice();
}

void Page::closeDevice()
{
    m_deviceController->closeDevice();
}

void Page::setupDevice(Device *device)
{
    connect(device, &Device::opened, this, &Page::onOpened);
    connect(device, &Device::closed, this, &Page::onClosed);
    connect(device, &Device::bytesWritten, this, &Page::onBytesWritten);
    connect(device, &Device::bytesRead, this, &Page::onBytesRead);
    connect(device, &Device::errorOccurred, this, &Page::onErrorOccurred);
    connect(device, &Device::warningOccurred, this, &::Page::onWarningOccurred);
    connect(ui->tabPreset, &PresetView::outputBytes, device, &Device::writeBytes);
    connect(ui->tabEmitter, &EmitterView::outputBytes, device, &Device::writeBytes);
    connect(ui->tabResponder, &ResponderView::outputBytes, device, &Device::writeBytes);
}

void Page::writeBytes()
{
    if (!m_deviceController->device()) {
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
        m_deviceController->device()->writeBytes(bytes);
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
    if (m_deviceController) {
        m_deviceController->setUiEnabled(enabled);
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
    bool isTerminal = ui->checkBoxTerminalMode->isChecked();
    int format = ui->comboBoxOutputFormat->currentData().toInt();

    if (isTerminal) {
        if (!isRx) {
            return;
        }

        QString text = bytes2string(bytes, format);
        ui->textBrowserOutput->moveCursor(QTextCursor::MoveOperation::End);
        ui->textBrowserOutput->insertPlainText(text);
        return;
    }

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
#if 0
    header = header.trimmed();
    header = QString("<font color=silver>[%1]</font>").arg(header);
    QString outputText = QString("%1 %2").arg(header, text);
    outputText = outputText.replace("\r", "\\r");
    outputText = outputText.replace("\n", "\\n");
#else
    header = QString("<font color=silver style='font-family: \"Segoe UI\", Arial; font-size: "
                     "12px;'>[%1]</font>")
                 .arg(header.trimmed());
    QString outputText
        = QString("<pre style='margin:0; font-family: \"JetBrains Mono\", Consolas, Monaco, "
                  "monospace; font-size: 13px; line-height: 1.1; padding: 2px 0;'>%1 %2</pre>")
              .arg(header, text);
#endif

    // 在添加到输出前进行搜索匹配
    if (!ui->lineEditSearch->text().isEmpty()) {
        performSearch(outputText);
    }

    if (m_outputSettings->isEnableFilter()) {
        QString filter = m_outputSettings->filterText();
        if (!outputText.contains(filter)) {
            return;
        }
    }
    ui->textBrowserOutput->append(outputText);
}

void Page::saveControllerParameters()
{
    if (m_deviceController) {
        auto parameters = m_deviceController->save();
        m_settings->setValue(m_deviceController->metaObject()->className(), parameters);
    }
}

void Page::loadControllerParameters()
{
    if (m_deviceController) {
        auto parameters = m_settings->value(m_deviceController->metaObject()->className());
        if (!parameters.isNull() && parameters.isValid()) {
            m_deviceController->load(parameters.toMap());
        }
    }
}

void Page::updateChartUi()
{
    ui->widgetCharts->setVisible(ui->toolButtonCharts->isChecked());
    ui->widgetChartsController->setVisible(ui->toolButtonCharts->isChecked());
}

void Page::onSearchButtonClicked()
{
    performSearch();
}

void Page::onSearchTextChanged()
{
    // 当搜索文本变化时，如果文本不为空，则执行搜索
    if (!ui->lineEditSearch->text().isEmpty()) {
        performSearch();
    } else {
        // 如果搜索文本为空，则清空搜索结果
        ui->textBrowserSearchResult->clear();
    }
}

void Page::performSearch()
{
    QString searchText = ui->lineEditSearch->text();
    if (searchText.isEmpty()) {
        return;
    }

    // 获取搜索选项
    bool useRegex = ui->checkBoxRegex->isChecked();
    bool matchCase = ui->checkBoxMatchCase->isChecked();
    bool wholeWord = ui->checkBoxWholeWord->isChecked();

    // 构建正则表达式
    QString pattern = searchText;
    if (!useRegex) {
        // 如果不使用正则表达式，则转义特殊字符
        pattern = QRegularExpression::escape(pattern);
    }

    // 如果需要全词匹配，则添加单词边界
    if (wholeWord) {
        pattern = QString("\\b%1\\b").arg(pattern);
    }

    // 创建正则表达式对象
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (!matchCase) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    QRegularExpression regex(pattern, options);

    // 获取textBrowserOutput的文本内容
    QString content = ui->textBrowserOutput->toPlainText();

    // 执行搜索并高亮结果
    highlightSearchResults(content, regex);
}

void Page::performSearch(QString &line)
{
    QString searchText = ui->lineEditSearch->text();
    if (searchText.isEmpty()) {
        return;
    }

    // 获取搜索选项
    bool useRegex = ui->checkBoxRegex->isChecked();
    bool matchCase = ui->checkBoxMatchCase->isChecked();
    bool wholeWord = ui->checkBoxWholeWord->isChecked();

    // 构建正则表达式
    QString pattern = searchText;
    if (!useRegex) {
        // 如果不使用正则表达式，则转义特殊字符
        pattern = QRegularExpression::escape(pattern);
    }

    // 如果需要全词匹配，则添加单词边界
    if (wholeWord) {
        pattern = QString("\\b%1\\b").arg(pattern);
    }

    // 创建正则表达式对象
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (!matchCase) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    QRegularExpression regex(pattern, options);

    // 只在当前行中搜索匹配项
    highlightSearchResultsForLine(line, regex);
}

void Page::highlightSearchResults(const QString &text, const QRegularExpression &regex)
{
    ui->textBrowserSearchResult->clear();

    // 查找所有匹配项
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(text);
    int matchCount = 0;

    // 存储所有匹配结果
    QList<QPair<int, QRegularExpressionMatch>> matches;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        matches.append(qMakePair(match.capturedStart(), match));
        matchCount++;
    }

    // 如果没有匹配项，显示提示信息
    if (matchCount == 0) {
        ui->textBrowserSearchResult->setHtml(QString("<p style='color:gray'>未找到匹配项</p>"));
        return;
    }

    // 显示匹配数量
    ui->textBrowserSearchResult->setHtml(
        QString("<p style='color:gray'>找到 %1 个匹配项</p>").arg(matchCount));

    // 获取文本的行
    QStringList lines = text.split('\n');

    // 对于每个匹配项，显示上下文
    for (const auto &matchPair : matches) {
        int matchPos = matchPair.first;
        QRegularExpressionMatch match = matchPair.second;

        // 计算匹配项所在的行号
        int lineNumber = 0;
        int posInLine = matchPos;
        for (int i = 0; i < lines.size(); i++) {
            if (posInLine <= lines[i].length()) {
                lineNumber = i;
                break;
            }
            posInLine -= lines[i].length() + 1; // +1 是为了换行符
        }

        // 获取匹配行及其上下文（前后各一行）
        QString contextBefore = lineNumber > 0 ? lines[lineNumber - 1] : QString();
        QString matchLine = lines[lineNumber];
        QString contextAfter = lineNumber < lines.size() - 1 ? lines[lineNumber + 1] : QString();

        // 在匹配行中高亮匹配文本
        QString matchedText = match.captured();
        int matchStartInLine = posInLine;
        int matchEndInLine = matchStartInLine + matchedText.length();

        QString highlightedLine = matchLine.left(matchStartInLine)
                                  + QString("<span style='background-color:yellow;'>%1</span>")
                                        .arg(matchedText)
                                  + matchLine.mid(matchEndInLine);

        // 构建HTML显示上下文
        QString html = "<div style='margin-bottom:10px;'>";
        html += QString("<p style='color:gray;margin:0;'>行 %1:</p>").arg(lineNumber + 1);

        if (!contextBefore.isEmpty()) {
            html += QString("<pre style='margin:0;color:gray;'>%1</pre>")
                        .arg(contextBefore.toHtmlEscaped());
        }

        html += QString("<pre style='margin:0;'>%1</pre>").arg(highlightedLine);

        if (!contextAfter.isEmpty()) {
            html += QString("<pre style='margin:0;color:gray;'>%1</pre>")
                        .arg(contextAfter.toHtmlEscaped());
        }

        html += "</div>";

        // 添加到搜索结果
        ui->textBrowserSearchResult->append(html);
    }
}

void Page::highlightSearchResultsForLine(const QString &line, const QRegularExpression &regex)
{
    // 查找所有匹配项
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(line);

    // 如果没有匹配项，直接返回
    if (!matchIterator.hasNext()) {
        return;
    }

    // 如果这是第一个匹配项，初始化搜索结果区域
    if (ui->textBrowserSearchResult->document()->isEmpty()) {
        ui->textBrowserSearchResult->setHtml(QString("<p style='color:gray'>实时搜索结果：</p>"));
    }

    // 使用原始文本进行匹配和高亮，与highlightSearchResults保持一致
    QString originalText = line;

    // 对于每个匹配项，高亮并添加到结果中
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString matchedText = match.captured();
        int matchStart = match.capturedStart();
        int matchEnd = matchStart + matchedText.length();

        // 在匹配行中高亮匹配文本，使用与highlightSearchResults相同的逻辑
        QString highlightedLine = originalText.left(matchStart)
                                  + QString("<span style='background-color:yellow;'>%1</span>")
                                        .arg(matchedText)
                                  + originalText.mid(matchEnd);

        // 构建HTML显示
        QString html = "<div style='margin-bottom:5px;'>";
        html += QString("<pre style='margin:0;'>%1</pre>").arg(highlightedLine);
        html += "</div>";

        // 添加到搜索结果
        ui->textBrowserSearchResult->append(html);

        // 更新匹配计数（可选）
        static int matchCount = 0;
        matchCount++;
        // 更新标题显示匹配数量
        QTextCursor cursor = ui->textBrowserSearchResult->textCursor();
        cursor.setPosition(0);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        cursor.insertHtml(
            QString("<p style='color:gray'>实时搜索结果：找到 %1 个匹配项</p>").arg(matchCount));
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
#endif
    case static_cast<int>(DeviceType::LocalSocket):
        return new LocalSocketUi();
    case static_cast<int>(DeviceType::LocalServer):
        return new LocalServerUi();
#ifdef X_ENABLE_CHARTS
    case static_cast<int>(DeviceType::ChartsTest):
        return new ChartsTestUi();
#endif
    default:
        return nullptr;
    }
}
