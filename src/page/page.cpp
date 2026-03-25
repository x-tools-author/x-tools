/***************************************************************************************************
 * Copyright 2024-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "page.h"
#include "ui_page.h"

#include <QButtonGroup>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QTimer>
#include <QToolButton>
#include <QVariantMap>
#include <QWidget>
#include <QWidgetAction>

#include "device/device.h"
#include "device/devicemanager.h"
#include "device/deviceui.h"

#include "page/datarecords/datarecordsview.h"
#include "page/emitter/emitterview.h"
#include "page/preset/presetpanel.h"
#include "page/preset/presetview.h"
#include "page/responder/responderview.h"
#include "page/scripts/scriptsmanager.h"
#include "page/search/searchpanel.h"
#include "page/transfer/transfersview.h"

#ifdef X_ENABLE_CHARTS
#include "page/charts/bar/barpanel.h"
#include "page/charts/line/linepanel.h"
#endif
#ifdef X_ENABLE_LUA
#include "page/lua/luapanel.h"
#include "page/lua/luaview.h"
#endif

#include "common/xtools.h"
#include "devicesettings.h"
#include "inputsettings.h"
#include "outputsettings.h"

#include "utilities/compatibility.h"
#include "utilities/crc.h"
#include "utilities/iconengine.h"
#include "utilities/statistician.h"
#include "utilities/syntaxhighlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Page;
}
QT_END_NAMESPACE

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
    const QString preset{"presetItems"};
    const QString emitterItems{"emitterItems"};
    const QString responserItems{"responserItems"};
    const QString transfers{"transfers"};
    const QString protocolFactory{"protocolFactory"};
    const QString dataRecords{"dataRecords"};
    const QString searchPanel{"searchPanel"};
    const QString barPanel{"barPanel"};
    const QString linePanel{"linePanel"};
    const QString scriptsManager{"scriptsView"};
    const QString luaView{"luaView"};
};

class PagePrivate : public QObject
{
public:
    explicit PagePrivate(Page *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    { }

    void initUi();
    void initUiDeviceControl();
    void initUiOutputControl();
    void initUiInputControl();
    void initUiOutput();
    void initUiInput();

    void onDeviceTypeChanged();
    void onCycleIntervalChanged();
    void onInputFormatChanged();
    void onOpenButtonClicked();
    void onHighlighterEnableChanged();
    void onHighlighterKeywordsChanged();
    void onShowStatisticianChanged(bool checked);

    void onOpened();
    void onClosed();
    void onErrorOccurred(const QString &error);
    void onWarningOccurred(const QString &warning);
    void onBytesRead(const QByteArray &bytes, const QString &from);
    void onBytesWritten(const QByteArray &bytes, const QString &to);
    void onWrapModeChanged();
    void onTerminalModeChanged();
    void onExternalPanelButtonClicked(bool checked);

    void openDevice();
    void closeDevice();
    void setupDevice(Device *device);
    void writeBytes();
    void writeSpecifiedBytes(const QByteArray &bytes);
    void updateLabelInfo();
    void setupMenu(QPushButton *target, QWidget *actionWidget);
    void setUiEnabled(bool enabled);
    void outputText(const QByteArray &bytes, const QString &flag, bool isRx);
    void saveControllerParameters();
    void loadControllerParameters();

    void addTab(const QString &name, QWidget *widget);
    void setTab(QWidget *widget);
    void removeTestDevices();

    QByteArray payload() const;
    QByteArray crc(const QByteArray &payload) const;

public:
    Ui::Page *ui{nullptr};
    DeviceUi *m_deviceController{nullptr};
    DeviceSettings *m_ioSettings{nullptr};
    OutputSettings *m_outputSettings{nullptr};
    InputSettings *m_inputSettings{nullptr};
    SyntaxHighlighter *m_highlighter{nullptr};
    Statistician *m_rxStatistician{nullptr};
    Statistician *m_txStatistician{nullptr};

    PresetPanel *m_presetPanel{nullptr};
    EmitterView *m_emitterView{nullptr};
    ResponderView *m_responderView{nullptr};
    TransfersView *m_transfersView{nullptr};
    ScriptsManager *m_scriptsManager{nullptr};
    DataRecordsView *m_dataRecordsView{nullptr};
    SearchPanel *m_filterView{nullptr};
    BarPanel *m_barPanel{nullptr};
    LinePanel *m_linePanel{nullptr};
    LuaView *m_luaView{nullptr};

    QList<QToolButton *> m_tabToolButtons;
    QTimer *m_writeTimer{nullptr};
    QTimer *m_updateLabelInfoTimer{nullptr};
    QSettings *m_settings{nullptr};

    QList<Panel *> m_panels;

private:
    Page *q;
};

void PagePrivate::initUi()
{
#if 0
    const QIcon icon = QIcon(":/res/icons/settings.svg");
    ui->pushButtonDeviceSettings->setIcon(icon);
    ui->pushButtonOutputSettings->setIcon(icon);
    ui->pushButtonInputSettings->setIcon(icon);
#endif
    ui->toolButtonInputPreset->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolButtonInputPreset->setStyleSheet("QToolButton::menu-indicator{image: none;}");

    initUiDeviceControl();
    initUiOutputControl();
    initUiInputControl();
    initUiOutput();
    initUiInput();
}

void PagePrivate::initUiDeviceControl()
{
    connect(ui->comboBoxDeviceTypes, xComboBoxActivated, this, &PagePrivate::onDeviceTypeChanged);
    connect(ui->pushButtonDeviceOpen,
            &QPushButton::clicked,
            this,
            &PagePrivate::onOpenButtonClicked);

    QPushButton *target = ui->pushButtonDeviceSettings;
    m_ioSettings = new DeviceSettings();
    setupMenu(target, m_ioSettings);

    xDevMgr.setupDeviceTypes(ui->comboBoxDeviceTypes);
}

void PagePrivate::initUiOutputControl()
{
    xSetupTextFormat(ui->comboBoxOutputFormat);
    ui->checkBoxOutputRx->setChecked(true);
    ui->checkBoxOutputTx->setChecked(true);
    ui->checkBoxOutputTime->setChecked(true);

    m_outputSettings = new OutputSettings();
    setupMenu(ui->pushButtonOutputSettings, m_outputSettings);

    connect(m_outputSettings,
            &OutputSettings::highlighterEnableChanged,
            this,
            &PagePrivate::onHighlighterEnableChanged);

    connect(m_outputSettings,
            &OutputSettings::highlighterKeywordsChanged,
            this,
            &PagePrivate::onHighlighterKeywordsChanged);
    connect(m_outputSettings,
            &OutputSettings::showStatisticianChanged,
            this,
            &PagePrivate::onShowStatisticianChanged);
    m_highlighter->setDocument(ui->textBrowserOutput->document());
}

void PagePrivate::initUiInputControl()
{
    connect(ui->comboBoxInputFormat, xComboBoxActivated, this, &PagePrivate::onInputFormatChanged);
    connect(ui->comboBoxInputInterval,
            xComboBoxActivated,
            this,
            &PagePrivate::onCycleIntervalChanged);
    connect(ui->pushButtonInputWriteBytes, &QPushButton::clicked, this, &PagePrivate::writeBytes);

    xSetupTextFormat(ui->comboBoxInputFormat);
    ui->comboBoxInputInterval->addItem(Page::tr("Disable"), -1);
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

void PagePrivate::initUiOutput()
{
    PresetView *presetView = m_presetPanel->presetView();
    ui->toolButtonInputPreset->setMenu(presetView->menu());
    ui->toolButtonInputPreset->setPopupMode(QToolButton::InstantPopup);
    ui->stackedWidget->setCurrentIndex(0);
    m_transfersView->setCurrentIndex(0);

    m_filterView->setOriginalTextBrowser(ui->textBrowserOutput);
}

void PagePrivate::initUiInput() { }

void PagePrivate::onDeviceTypeChanged()
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

void PagePrivate::onCycleIntervalChanged()
{
    int interval = ui->comboBoxInputInterval->currentData().toInt();
    if (interval > 0) {
        m_writeTimer->start(interval);
    } else {
        m_writeTimer->stop();
    }
}

void PagePrivate::onInputFormatChanged()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    QString placeholderText = xBytes2string(QByteArray("(null)"), format);
    xSetupTextFormatValidator(ui->lineEditInput, format);
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

    ui->lineEditInput->setVisible(usingLineEdit);
    ui->plainTextEditInput->setVisible(!usingLineEdit);
}

void PagePrivate::onOpenButtonClicked()
{
    ui->pushButtonDeviceOpen->setEnabled(false);
    if (m_deviceController->device() && m_deviceController->device()->isRunning()) {
        closeDevice();
    } else {
        openDevice();
    }
}

void PagePrivate::onHighlighterEnableChanged()
{
    bool enabled = m_outputSettings->isEnableHighlighter();
    m_highlighter->setEnabled(enabled);
}

void PagePrivate::onHighlighterKeywordsChanged()
{
    QStringList keywords = m_outputSettings->highlighterKeywords();
    m_highlighter->setKeywords(keywords);
}

void PagePrivate::onShowStatisticianChanged(bool checked)
{
    ui->widgetRxTxInfo->setVisible(checked);
}

void PagePrivate::onOpened()
{
#if X_ENABLE_CHARTS
    m_linePanel->resetChart();
    m_barPanel->resetChart();
#endif

    setUiEnabled(false);
    onCycleIntervalChanged();

    ui->pushButtonDeviceOpen->setEnabled(true);
    ui->pushButtonDeviceOpen->setText(Page::tr("Close"));
}

void PagePrivate::onClosed()
{
    m_writeTimer->stop();

    setUiEnabled(true);
    ui->pushButtonDeviceOpen->setEnabled(true);
    ui->pushButtonDeviceOpen->setText(Page::tr("Open"));
}

void PagePrivate::onErrorOccurred(const QString &error)
{
    closeDevice();
    if (!error.isEmpty()) {
        QMessageBox::warning(q, Page::tr("Error Occurred"), error);
    }
}

void PagePrivate::onWarningOccurred(const QString &warning)
{
    QMessageBox::warning(q, Page::tr("Warning"), warning);
}

void PagePrivate::onBytesRead(const QByteArray &bytes, const QString &from)
{
    QByteArray cookedBytes = bytes;
    QString cookedFrom = from;
#if X_ENABLE_LUA
    LuaPanel *luaPanel = m_luaView->outputPanel();
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

    // Notify panels
    for (Panel *panel : m_panels) {
        panel->onBytesRead(cookedBytes, cookedFrom);
    }

    emit q->bytesRead(cookedBytes, cookedFrom);
}

void PagePrivate::onBytesWritten(const QByteArray &bytes, const QString &to)
{
    // Notify panels
    for (Panel *panel : m_panels) {
        panel->onBytesWritten(bytes, to);
    }

    m_ioSettings->saveData(bytes, true);
    m_txStatistician->inputBytes(bytes);
    outputText(bytes, to, false);
    emit q->bytesWritten(bytes, to);
}

void PagePrivate::onWrapModeChanged()
{
    if (ui->checkBoxWrap->isChecked()) {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::WrapAnywhere);
        ui->textBrowserOutput->setLineWrapMode(QTextEdit::WidgetWidth);
    } else {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::NoWrap);
        ui->textBrowserOutput->setLineWrapMode(QTextEdit::NoWrap);
    }
}

void PagePrivate::onTerminalModeChanged()
{
    bool terminalMode = ui->checkBoxTerminalMode->isChecked();
    ui->checkBoxOutputRx->setEnabled(!terminalMode);
    ui->checkBoxOutputTx->setEnabled(!terminalMode);
    ui->checkBoxOutputFlag->setEnabled(!terminalMode);
    ui->checkBoxOutputDate->setEnabled(!terminalMode);
    ui->checkBoxOutputTime->setEnabled(!terminalMode);
    ui->checkBoxOutputMs->setEnabled(!terminalMode);

    m_filterView->setWholeWordCheckBoxEnabled(terminalMode);
}

void PagePrivate::onExternalPanelButtonClicked(bool checked) { }

void PagePrivate::openDevice()
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

void PagePrivate::closeDevice()
{
    m_deviceController->closeDevice();
}

void PagePrivate::setupDevice(Device *device)
{
    if (!device) {
        return;
    }

    connect(device, &Device::opened, this, &PagePrivate::onOpened);
    connect(device, &Device::closed, this, &PagePrivate::onClosed);
    connect(device, &Device::bytesWritten, this, &PagePrivate::onBytesWritten);
    connect(device, &Device::bytesRead, this, &PagePrivate::onBytesRead);
    connect(device, &Device::errorOccurred, this, &PagePrivate::onErrorOccurred);
    connect(device, &Device::warningOccurred, this, &PagePrivate::onWarningOccurred);
}

void PagePrivate::writeBytes()
{
    if (!m_deviceController->device()) {
        return;
    }

    InputSettings::Parameters parameters = m_inputSettings->parameters();
    QByteArray prefix = xCookedAffixes(parameters.prefix);
    if (parameters.usingCustomPrefix) {
        prefix = parameters.customPrefix;
    }
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = xCookedAffixes(parameters.suffix);
    if (parameters.usingCustomSuffix) {
        suffix = parameters.customSuffix;
    }

    QByteArray bytes;
    if (parameters.appendCrc) {
        bytes = prefix + payload + crc + suffix;
    } else {
        bytes = prefix + payload + suffix;
    }

    if (!bytes.isEmpty()) {
#if X_ENABLE_LUA
        LuaPanel *luaPanel = m_luaView->inputPanel();
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

void PagePrivate::writeSpecifiedBytes(const QByteArray &bytes)
{
    auto device = m_deviceController ? m_deviceController->device() : nullptr;
    if (device && device->isRunning()) {
        device->writeBytes(bytes);
    } else {
        QString msg = Page::tr("Error: No device is opened.");
        ui->textBrowserOutput->append(QString("<span style=\"color:#d24373;\">%1</span>").arg(msg));
    }
}

void PagePrivate::updateLabelInfo()
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();

    QByteArray prefix = xCookedAffixes(parameters.prefix);
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = xCookedAffixes(parameters.suffix);

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
    QString info = Page::tr("[%1][%2][%3][%4]")
                       .arg(prefixString, payloadString, crcString, suffixString);
    QFontMetrics fontWidth(ui->labelInfo->font());
    info = fontWidth.elidedText(info, Qt::ElideMiddle, ui->labelInfo->width());
    if (parameters.showDataPreview) {
        ui->labelInfo->setText(info);
    } else {
        ui->labelInfo->clear();
    }
}

void PagePrivate::setupMenu(QPushButton *target, QWidget *actionWidget)
{
    QMenu *menu = new QMenu(target);
    QWidgetAction *action = new QWidgetAction(menu);
    action->setDefaultWidget(actionWidget);
    menu->addAction(action);
    target->setMenu(menu);
}

void PagePrivate::setUiEnabled(bool enabled)
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

void PagePrivate::outputText(const QByteArray &bytes, const QString &flag, bool isRx)
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

        QString text = xBytes2string(bytes, format);
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
    QString text = xBytes2string(bytes, format);
    QString rxTx = isRx ? QStringLiteral("Rx") : QStringLiteral("Tx");
    rxTx = QString("<font color=%1>%2</font>").arg(isRx ? "blue" : "green", rxTx);

    QString header;
    if (showFlag) {
        header = QString("%1 %2 %3").arg(rxTx, dateTimeString, flag);
    } else {
        header = QString("%1 %2").arg(rxTx, dateTimeString);
    }

#if 1
    header = header.trimmed();
    header = QString("<font color=silver>[%1]</font>").arg(header);
    QString outputText = QString("%1 %2").arg(header, text);
    if (m_outputSettings->showRN()) {
        outputText = outputText.replace("\r", "\\r");
        outputText = outputText.replace("\n", "\\n");
    }
#else
    // The compatibility is too poor.
    header = QString("<font color=silver style='font-family: \"Segoe UI\", Arial; font-size: "
                     "12px;'>[%1]</font>")
                 .arg(header.trimmed());
    QString outputText
        = QString("<pre style='margin:0; font-family: \"JetBrains Mono\", Consolas, Monaco, "
                  "monospace; font-size: 13px; line-height: 1.1; padding: 2px 0;'>%1 %2</pre>")
              .arg(header, text);
#endif

    if (m_outputSettings->isEnableFilter()) {
        QString filter = m_outputSettings->filterText();
        if (!outputText.contains(filter)) {
            return;
        }
    }
    ui->textBrowserOutput->append(outputText);
    m_filterView->performSearch(outputText);
}

void PagePrivate::saveControllerParameters()
{
    if (m_deviceController) {
        auto parameters = m_deviceController->save();
        m_settings->setValue(m_deviceController->metaObject()->className(), parameters);
    }
}

void PagePrivate::loadControllerParameters()
{
    if (m_deviceController) {
        auto parameters = m_settings->value(m_deviceController->metaObject()->className());
        if (!parameters.isNull() && parameters.isValid()) {
            m_deviceController->load(parameters.toMap());
        }
    }
}

void PagePrivate::addTab(const QString &name, QWidget *widget)
{
    QToolButton *button = new QToolButton();
    button->setText(name);
    ui->horizontalLayoutTab->addWidget(button);
    ui->stackedWidget->addWidget(widget);

    button->setCheckable(true);
    connect(button, &QToolButton::clicked, this, [this, button, widget](bool checked) {
        this->setTab(checked ? widget : nullptr);
        for (auto &btn : m_tabToolButtons) {
            if (btn != button) {
                btn->setChecked(false);
            }
        }
    });
    m_tabToolButtons.append(button);
}

void PagePrivate::setTab(QWidget *widget)
{
    if (widget) {
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentWidget(widget);
    } else {
        ui->stackedWidget->hide();
    }
}

void PagePrivate::removeTestDevices()
{
    for (int i = 0; i < ui->comboBoxDeviceTypes->count(); i++) {
        int type = ui->comboBoxDeviceTypes->itemData(i).toInt();
        if (type == static_cast<int>(DeviceManager::ChartsTest)) {
            ui->comboBoxDeviceTypes->removeItem(i);
            break;
        }
    }
}

QByteArray PagePrivate::payload() const
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
    text = xCookedEscapeCharacter(text, parameters.escapeCharacter);
    QByteArray payload = xString2bytes(text, format);
    return payload;
}

QByteArray PagePrivate::crc(const QByteArray &payload) const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    xTools::CRC::Context ctx;
    ctx.algorithm = static_cast<xTools::CRC::Algorithm>(parameters.algorithm);
    ctx.startIndex = parameters.startIndex;
    ctx.endIndex = parameters.endIndex;
    ctx.bigEndian = parameters.bigEndian;
    ctx.data = payload;

    return xTools::CRC::calculate(ctx);
}

Page::Page(ControllerDirection direction, QSettings *settings, QWidget *parent)
    : QWidget{parent}
{
    d = new PagePrivate(this);
    d->ui = new Ui::Page;
    d->m_highlighter = new SyntaxHighlighter(this);
    d->m_writeTimer = new QTimer(this);
    d->m_updateLabelInfoTimer = new QTimer(this);
    d->m_settings = settings;

    if (settings == nullptr) {
        static QSettings defaultSettings("IO_Page.ini", QSettings::IniFormat);
        d->m_settings = &defaultSettings;
    }

    d->ui->setupUi(this);
    d->ui->stackedWidget->hide();

    d->m_rxStatistician = new Statistician(d->ui->labelRxInfo, this);
    d->m_txStatistician = new Statistician(d->ui->labelTxInfo, this);
    d->m_presetPanel = new PresetPanel(this);
    d->m_emitterView = new EmitterView(this);
    d->m_responderView = new ResponderView(this);
    d->m_transfersView = new TransfersView(this);
    d->m_dataRecordsView = new DataRecordsView(this);
    d->m_filterView = new SearchPanel(this);

    d->addTab(tr("Presets"), d->m_presetPanel);
    d->addTab(tr("Emitter"), d->m_emitterView);
    d->addTab(tr("Responder"), d->m_responderView);
    d->addTab(tr("Transfers"), d->m_transfersView);
    d->addTab(tr("Records"), d->m_dataRecordsView);
    d->addTab(tr("Search"), d->m_filterView);

#ifdef X_ENABLE_CHARTS
    d->m_barPanel = new BarPanel(this);
    d->m_linePanel = new LinePanel(this);
    d->addTab(tr("Bar Charts"), d->m_barPanel);
    d->addTab(tr("Line Charts"), d->m_linePanel);
#endif
    d->m_scriptsManager = new ScriptsManager(this);
    d->addTab(tr("Scripts"), d->m_scriptsManager);
#ifdef X_ENABLE_LUA
    d->m_luaView = new LuaView(this);
    d->addTab(QString("Lua"), d->m_luaView);
#endif

    if (direction == ControllerDirection::Right) {
        QHBoxLayout *l = qobject_cast<QHBoxLayout *>(layout());
        if (l) {
            auto item = l->takeAt(0);
            l->addItem(item);
        }
    }

    d->m_writeTimer->setInterval(1000);
    connect(d->m_writeTimer, &QTimer::timeout, d, &PagePrivate::writeBytes);

    // TODO: Do not use timer to update label info, use signal-slot mechanism instead.
    d->m_updateLabelInfoTimer->setInterval(100);
    connect(d->m_updateLabelInfoTimer, &QTimer::timeout, d, &PagePrivate::updateLabelInfo);
    d->m_updateLabelInfoTimer->start();

    // clang-format off
    connect(d->ui->lineEditInput, &QLineEdit::returnPressed, d, &PagePrivate::writeBytes);
    connect(d->ui->checkBoxWrap, &QCheckBox::clicked, d, &PagePrivate::onWrapModeChanged);
    connect(d->ui->checkBoxTerminalMode, &QCheckBox::clicked, d, &PagePrivate::onTerminalModeChanged);
    // clang-format on

    d->initUi();
    d->onShowStatisticianChanged(false);
    d->onDeviceTypeChanged();
    d->onTerminalModeChanged();
    d->onInputFormatChanged();
    d->onExternalPanelButtonClicked(false);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    d->ui->widgetController->setMaximumWidth(256);
#endif
}

Page::~Page()
{
    d->m_updateLabelInfoTimer->stop();
    d->saveControllerParameters();
    delete d->ui;
    d = nullptr;
}

QVariantMap Page::save() const
{
    QVariantMap map;
    ParameterKeys keys;
    // Communication settings
    map.insert(keys.communicationType, d->ui->comboBoxDeviceTypes->currentData());
    map.insert(keys.communicationSettings, d->m_ioSettings->save());
    if (d->m_deviceController) {
        map.insert(keys.communication, d->m_deviceController->save());
    }

    // Output settings
    map.insert(keys.outputFormat, d->ui->comboBoxOutputFormat->currentData());
    map.insert(keys.outputRx, d->ui->checkBoxOutputRx->isChecked());
    map.insert(keys.outputTx, d->ui->checkBoxOutputTx->isChecked());
    map.insert(keys.outputFlag, d->ui->checkBoxOutputFlag->isChecked());
    map.insert(keys.outputDate, d->ui->checkBoxOutputDate->isChecked());
    map.insert(keys.outputTime, d->ui->checkBoxOutputTime->isChecked());
    map.insert(keys.outputMs, d->ui->checkBoxOutputMs->isChecked());
    map.insert(keys.outputSettings, d->m_outputSettings->save());
    map.insert(keys.outputWrap, d->ui->checkBoxWrap->isChecked());
    map.insert(keys.outputTerminalMode, d->ui->checkBoxTerminalMode->isChecked());

    // Input settings
    map.insert(keys.cycleInterval, d->ui->comboBoxInputInterval->currentData());
    map.insert(keys.inputFormat, d->ui->comboBoxInputFormat->currentData());
    map.insert(keys.inputSettings, d->m_inputSettings->save());

    // Tabs
    int index = -1;
    for (QToolButton *btn : d->m_tabToolButtons) {
        if (btn->isChecked()) {
            index = d->m_tabToolButtons.indexOf(btn);
            break;
        }
    }
    map.insert(keys.tabIndex, index);
    map.insert(keys.preset, d->m_presetPanel->save());
    map.insert(keys.emitterItems, d->m_emitterView->save());
    map.insert(keys.responserItems, d->m_responderView->save());
    map.insert(keys.transfers, d->m_transfersView->save());
    map.insert(keys.dataRecords, d->m_dataRecordsView->save());
    map.insert(keys.searchPanel, d->m_filterView->save());
    map.insert(keys.scriptsManager, d->m_scriptsManager->save());
#ifdef X_ENABLE_CHARTS
    map.insert(keys.barPanel, d->m_barPanel->save());
    map.insert(keys.linePanel, d->m_linePanel->save());
#endif
#ifdef X_ENABLE_LUA
    map.insert(keys.luaView, d->m_luaView->save());
#endif

    return map;
}

void Page::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    ParameterKeys keys;

    // Communication settings
    int communicationType = parameters.value(keys.communicationType).toInt();
    int index = d->ui->comboBoxDeviceTypes->findData(communicationType);
    QVariantMap communicationSettings = parameters.value(keys.communicationSettings).toMap();
    d->m_ioSettings->load(communicationSettings);

    d->ui->comboBoxDeviceTypes->setCurrentIndex(index == -1 ? 0 : index);
    if (d->m_deviceController) {
        d->m_deviceController->load(parameters.value(keys.communication).toMap());
    }

    // Output settings
    int outputFormat = parameters.value(keys.outputFormat).toInt();
    bool outputRx = parameters.value(keys.outputRx).toBool();
    bool outputTx = parameters.value(keys.outputTx).toBool();
    bool outputFlag = parameters.value(keys.outputFlag).toBool();
    bool outputDate = parameters.value(keys.outputDate).toBool();
    bool outputTime = parameters.value(keys.outputTime).toBool();
    bool outputMs = parameters.value(keys.outputMs).toBool();
    bool outputWrap = parameters.value(keys.outputWrap).toBool();
    bool outputTerminalMode = parameters.value(keys.outputTerminalMode).toBool();
    QVariantMap outputSettings = parameters.value(keys.outputSettings).toMap();

    index = d->ui->comboBoxOutputFormat->findData(outputFormat);
    d->ui->comboBoxOutputFormat->setCurrentIndex(index == -1 ? 0 : index);
    d->ui->checkBoxOutputRx->setChecked(outputRx);
    d->ui->checkBoxOutputTx->setChecked(outputTx);
    d->ui->checkBoxOutputFlag->setChecked(outputFlag);
    d->ui->checkBoxOutputDate->setChecked(outputDate);
    d->ui->checkBoxOutputTime->setChecked(outputTime);
    d->ui->checkBoxOutputMs->setChecked(outputMs);
    d->ui->checkBoxWrap->setChecked(outputWrap);
    d->ui->checkBoxTerminalMode->setChecked(outputTerminalMode);
    d->m_outputSettings->load(outputSettings);

    // Input settings
    int inputInterval = parameters.value(keys.cycleInterval).toInt();
    int inputFormat = parameters.value(keys.inputFormat).toInt();
    QVariantMap inputSettings = parameters.value(keys.inputSettings).toMap();

    index = d->ui->comboBoxInputInterval->findData(inputInterval);
    d->ui->comboBoxInputInterval->setCurrentIndex(index == -1 ? 0 : index);
    index = d->ui->comboBoxInputFormat->findData(inputFormat);
    d->ui->comboBoxInputFormat->setCurrentIndex(index == -1 ? 0 : index);
    d->m_inputSettings->load(inputSettings);

    // Load Tabs
    int tabIndex = parameters.value(keys.tabIndex, -1).toInt();
    if (tabIndex >= 0 && tabIndex < d->m_tabToolButtons.size()) {
        d->m_tabToolButtons[tabIndex]->setChecked(true);
        d->ui->stackedWidget->setCurrentIndex(tabIndex);
        d->ui->stackedWidget->show();
    }
    d->m_presetPanel->load(parameters.value(keys.preset).toMap());
    d->m_emitterView->load(parameters.value(keys.emitterItems).toMap());
    d->m_responderView->load(parameters.value(keys.responserItems).toMap());
    d->m_transfersView->load(parameters.value(keys.transfers).toMap());
    d->m_dataRecordsView->load(parameters.value(keys.dataRecords).toMap());
    d->m_filterView->load(parameters.value(keys.searchPanel).toMap());
#ifdef X_ENABLE_CHARTS
    d->m_barPanel->load(parameters.value(keys.barPanel).toMap());
    d->m_linePanel->load(parameters.value(keys.linePanel).toMap());
#endif
    d->m_scriptsManager->load(parameters.value(keys.scriptsManager).toJsonObject());
#ifdef X_ENABLE_LUA
    d->m_luaView->load(parameters.value(keys.luaView).toMap());
#endif

    d->m_panels.append(d->m_presetPanel);
    //d->m_panels.append(d->m_emitterView);
    //d->m_panels.append(d->m_responderView);
    //d->m_panels.append(d->m_transfersView);
    d->m_panels.append(d->m_dataRecordsView);
#ifdef X_ENABLE_CHARTS
    d->m_panels.append(d->m_barPanel);
    d->m_panels.append(d->m_linePanel);
#endif
    //d->m_panels.append(d->m_scriptsManager);
#ifdef X_ENABLE_LUA
    d->m_panels.append(d->m_luaView);
#endif
    for (Panel *panel : d->m_panels) {
        connect(panel, &Panel::outputBytes, d, &PagePrivate::writeSpecifiedBytes);
    }

    d->onDeviceTypeChanged();
    d->onInputFormatChanged();
    d->onWrapModeChanged();
}

void Page::writeBytes(const QByteArray &bytes)
{
    auto device = d->m_deviceController ? d->m_deviceController->device() : nullptr;
    if (device && device->isRunning()) {
        device->writeBytes(bytes);
    } else {
        QString msg = tr("Error: No device is opened.");
        d->ui->textBrowserOutput->append(
            QString("<span style=\"color:#FF0000;\">%1</span>").arg(msg));
    }
}

void Page::prependOutputControl(QWidget *widget)
{
    d->ui->horizontalLayoutOutput->insertWidget(0, widget);
}

void Page::appendOutputControl(QWidget *widget)
{
    d->ui->horizontalLayoutOutput->addWidget(widget);
}

void Page::aboutToClose()
{
    d->m_scriptsManager->aboutToClose();
}

void hideAllWidgets(QHBoxLayout *layout)
{
    int count = layout->count();
    for (int i = count - 1; i >= 0; i--) {
        QLayoutItem *item = layout->itemAt(i);
        if (item) {
            QWidget *w = item->widget();
            if (w) {
                w->hide();
            }
        }
    }
}

void Page::showLiteMode()
{
    d->removeTestDevices();
    hideAllWidgets(d->ui->horizontalLayoutTab);
}
