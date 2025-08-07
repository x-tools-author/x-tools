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

#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QWidgetAction>

#include "device/device.h"
#include "device/devicemanager.h"
#include "device/deviceui.h"

#include "devicesettings.h"
#include "emitter/emitterview.h"
#include "page/panels/inputpanels/inputpanelsmanager.h"
#include "page/panels/outputpanels/search/searchpanel.h"
#include "page/preset/presetview.h"
#include "page/responder/responderview.h"

#ifdef X_ENABLE_CHARTS
#include "page/panels/outputpanels/charts/bar/barpanel.h"
#include "page/panels/outputpanels/charts/line/linepanel.h"
#endif

#ifdef X_ENABLE_LUA
#include "page/panels/common/luapanel.h"
#endif

#include "common/crc.h"
#include "common/xtools.h"
#include "inputsettings.h"
#include "outputsettings.h"
#include "utilities/statistician.h"
#include "utilities/syntaxhighlighter.h"

struct ParameterKeys
{
    // Communication settings
    const QString communicationType{"communicationType"};
    const QString communicationSettings{"communicationSettings"};
    const QString communication{"communication"};

    // Output settings
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

    // Input settings
    const QString cycleInterval{"cycleInterval"};
    const QString inputFormat{"inputFormat"};
    const QString inputSettings{"inputSettings"};

    // Tabs
    const QString tabIndex{"tabIndex"};
    const QString presetItems{"presetItems"};
    const QString emitterItems{"emitterItems"};
    const QString responserItems{"responserItems"};
    const QString transfers{"transfers"};

    // Panels
    const QString inputPanels{"inputPanels"};
    const QString outputPanels{"outputPanels"};
} g_keys;

Page::Page(ControllerDirection direction, QSettings *settings, QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::Page}
    , m_deviceController{nullptr}
    , m_ioSettings{nullptr}
    , m_outputSettings{nullptr}
    , m_inputSettings{nullptr}
    , m_highlighter{new SyntaxHighlighter(this)}
    , m_writeTimer{new QTimer(this)}
    , m_updateLabelInfoTimer{new QTimer(this)}
    , m_settings{settings}
{
    if (settings == nullptr) {
        m_settings = new QSettings("IO_Page.ini", QSettings::IniFormat);
    }

    ui->setupUi(this);
    m_rxStatistician = new Statistician(ui->labelRxInfo, this);
    m_txStatistician = new Statistician(ui->labelTxInfo, this);

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

    connect(ui->lineEditInput, &QLineEdit::returnPressed, this, &Page::writeBytes);
    connect(ui->checkBoxWrap, &QCheckBox::clicked, this, &Page::onWrapModeChanged);
    connect(ui->checkBoxTerminalMode, &QCheckBox::clicked, this, &Page::onTerminalModeChanged);
    connect(ui->widgetInputPanels,
            &InputPanelsManager::visibleChanged,
            this,
            &Page::onInputFormatChanged);

    initUi();

    onShowStatisticianChanged(false);
    onDeviceTypeChanged();
    onTerminalModeChanged();
    onInputFormatChanged();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    ui->widgetController->setMaximumWidth(256);
#endif
}

Page::~Page()
{
    m_updateLabelInfoTimer->stop();
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

    map.insert(g_keys.tabIndex, ui->tabWidget->currentIndex());
    map.insert(g_keys.presetItems, ui->tabPreset->save());
    map.insert(g_keys.emitterItems, ui->tabEmitter->save());
    map.insert(g_keys.responserItems, ui->tabResponder->save());

    map.insert(g_keys.inputPanels, ui->widgetInputPanels->save());
    map.insert(g_keys.outputPanels, ui->widgetOutputPanels->save());

    if (ui->tabTransfers->isEnabled()) {
        map.insert(g_keys.transfers, ui->tabTransfers->save());
    }

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

    ui->tabWidget->setCurrentIndex(parameters.value(g_keys.tabIndex, 0).toInt());
    ui->tabPreset->load(parameters.value(g_keys.presetItems).toMap());
    ui->tabEmitter->load(parameters.value(g_keys.emitterItems).toMap());
    ui->tabResponder->load(parameters.value(g_keys.responserItems).toMap());
    ui->tabTransfers->load(parameters.value(g_keys.transfers).toMap());

    ui->widgetInputPanels->load(parameters.value(g_keys.inputPanels).toMap());
    ui->widgetOutputPanels->load(parameters.value(g_keys.outputPanels).toMap());

    onDeviceTypeChanged();
    onInputFormatChanged();
    onWrapModeChanged();
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
        if (type == static_cast<int>(DeviceManager::ChartsTest)) {
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
    connect(ui->comboBoxDeviceTypes, xComboBoxActivated, this, &Page::onDeviceTypeChanged);
    connect(ui->pushButtonDeviceOpen, &QPushButton::clicked, this, &Page::onOpenButtonClicked);

    QPushButton *target = ui->pushButtonDeviceSettings;
    m_ioSettings = new DeviceSettings();
    setupMenu(target, m_ioSettings);

    xDevMgr.setupDeviceTypes(ui->comboBoxDeviceTypes);
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
    connect(ui->comboBoxInputFormat, xComboBoxActivated, this, &Page::onInputFormatChanged);
    connect(ui->comboBoxInputInterval, xComboBoxActivated, this, &Page::onCycleIntervalChanged);
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

    SearchPanel *searchPanel = ui->widgetOutputPanels->getPanel<SearchPanel>();
    if (searchPanel) {
        searchPanel->setOriginalTextBrowser(ui->textBrowserOutput);
    }

    QList<QToolButton *> buttons = ui->widgetOutputPanels->buttons();
    for (auto &button : buttons) {
        ui->horizontalLayoutOutputPanelsController->addWidget(button);
    }
}

void Page::initUiInput()
{
    QList<QToolButton *> buttons = ui->widgetInputPanels->buttons();
    for (auto &button : buttons) {
        ui->horizontalLayoutInputPanelsController->addWidget(button);
    }
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
    m_deviceController = xDevMgr.newDeviceUi(type);
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
    QString placeholderText = bytes2string(QByteArray("(null)"), format);
    setupTextFormatValidator(ui->lineEditInput, format);
    ui->lineEditInput->clear();
    ui->lineEditInput->setPlaceholderText(placeholderText);
    ui->plainTextEditInput->setPlaceholderText(placeholderText);

    bool usingLineEdit = format == static_cast<int>(TextFormat::Bin);
    usingLineEdit |= format == static_cast<int>(TextFormat::Oct);
    usingLineEdit |= format == static_cast<int>(TextFormat::Dec);
    usingLineEdit |= format == static_cast<int>(TextFormat::Hex);
    usingLineEdit |= format == static_cast<int>(TextFormat::HexWithoutSpace);

    ui->lineEditInput->setEnabled(usingLineEdit);
    ui->plainTextEditInput->setEnabled(!usingLineEdit);
    ui->lineEditInput->setVisible(true);
    ui->plainTextEditInput->setVisible(true);

    // If the input panels are visible, hide the line edit or plain text edit
    // to make ui simple.
    bool rightPanelVisible = ui->widgetInputPanels->isVisible();
    if (rightPanelVisible) {
        ui->lineEditInput->setVisible(true);
        ui->plainTextEditInput->setVisible(true);
    } else {
        ui->lineEditInput->setVisible(usingLineEdit);
        ui->plainTextEditInput->setVisible(!usingLineEdit);
    }
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
#if X_ENABLE_CHARTS
    LinePanel *linePanel = ui->widgetOutputPanels->getPanel<LinePanel>();
    if (linePanel) {
        linePanel->resetChart();
    }

    BarPanel *barPanel = ui->widgetOutputPanels->getPanel<BarPanel>();
    if (barPanel) {
        barPanel->resetChart();
    }
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
    QByteArray cookedBytes = bytes;
    QString cookedFrom = from;
#ifdef X_ENABLE_LUA
    LuaPanel *luaPanel = ui->widgetOutputPanels->getPanel<LuaPanel>();
    if (luaPanel && !luaPanel->isBypassed()) {
        QByteArray result = luaPanel->handleData(bytes);
        if (!result.isEmpty()) {
            cookedBytes = result;
            cookedFrom = from + " (Lua)";
        }
    }
#endif

    m_ioSettings->saveData(cookedBytes, false);
    m_rxStatistician->inputBytes(cookedBytes);
    outputText(cookedBytes, cookedFrom, true);

    ui->widgetOutputPanels->inputBytes(cookedBytes);
    ui->tabResponder->inputBytes(cookedBytes);

    if (ui->tabTransfers->isEnabled()) {
        ui->tabTransfers->inputBytes(cookedBytes);
    }

    emit bytesRead(cookedBytes, cookedFrom);
}

void Page::onBytesWritten(const QByteArray &bytes, const QString &to)
{
    QByteArray cookedBytes = bytes;
    QString cookedTo = to;
#ifdef X_ENABLE_LUA
    LuaPanel *luaPanel = ui->widgetOutputPanels->getPanel<LuaPanel>();
    if (luaPanel && !luaPanel->isBypassed()) {
        QByteArray result = luaPanel->handleData(bytes);
        if (!result.isEmpty()) {
            cookedBytes = result;
            cookedTo = to + " (Lua)";
        }
    }
#endif

    m_ioSettings->saveData(cookedBytes, true);
    m_txStatistician->inputBytes(cookedBytes);
    outputText(cookedBytes, cookedTo, false);

    emit bytesWritten(cookedBytes, cookedTo);
}

void Page::onWrapModeChanged()
{
    if (ui->checkBoxWrap->isChecked()) {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::WrapAnywhere);
        ui->textBrowserOutput->setLineWrapMode(QTextEdit::WidgetWidth);
    } else {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::NoWrap);
        ui->textBrowserOutput->setLineWrapMode(QTextEdit::NoWrap);
    }
}

void Page::onTerminalModeChanged()
{
    bool terminalMode = ui->checkBoxTerminalMode->isChecked();
    ui->checkBoxOutputRx->setEnabled(!terminalMode);
    ui->checkBoxOutputTx->setEnabled(!terminalMode);
    ui->checkBoxOutputFlag->setEnabled(!terminalMode);
    ui->checkBoxOutputDate->setEnabled(!terminalMode);
    ui->checkBoxOutputTime->setEnabled(!terminalMode);
    ui->checkBoxOutputMs->setEnabled(!terminalMode);

    SearchPanel *searchPanel = ui->widgetOutputPanels->getPanel<SearchPanel>();
    if (searchPanel) {
        searchPanel->setWholeWordCheckBoxEnabled(terminalMode);
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
    if (!device) {
        return;
    }

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
#ifdef X_ENABLE_LUA
        LuaPanel *luaPanel = ui->widgetInputPanels->getPanel<LuaPanel>();
        if (luaPanel) {
            QByteArray result = luaPanel->handleData(bytes);
            if (!result.isEmpty()) {
                bytes = result;
            }
        }
#endif
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

#if 1
    // 在添加到输出前进行搜索匹配
    SearchPanel *searchPanel = ui->widgetOutputPanels->getPanel<SearchPanel>();
    if (searchPanel) {
        searchPanel->performSearch(outputText);
    }
#endif

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

QByteArray Page::payload() const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    QString text;
    if (ui->lineEditInput->isEnabled()) {
        text = ui->lineEditInput->text();
        if (text.isEmpty()) {
            text = ui->lineEditInput->placeholderText();
        }
    } else {
        text = ui->plainTextEditInput->toPlainText();
        if (text.isEmpty()) {
            text = ui->plainTextEditInput->placeholderText();
        }
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
