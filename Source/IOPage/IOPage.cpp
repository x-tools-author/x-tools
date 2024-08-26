/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "IOPage.h"
#include "ui_IOPage.h"

#include <QMenu>
#include <QMessageBox>
#include <QWidgetAction>

#include "IO/IO/Communication/Communication.h"
#include "IO/IO/IOFactory.h"
#include "IO/IO/Model/Emitter.h"
#include "IO/IO/Model/Preset.h"
#include "IO/IO/Model/Responder.h"
#include "IO/IO/Processor/Statistician.h"
#include "IO/IO/Transfer/TcpClientTransfer.h"
#include "IO/IO/Transfer/TcpServerTransfer.h"
#include "IO/IO/Transfer/UdpClientTransfer.h"
#include "IO/IO/Transfer/WebSocketClientTransfer.h"
#include "IO/IO/Transfer/WebSocketServerTransfer.h"
#include "IO/UI/Communication/CommunicationUi.h"
#include "IO/UI/IOUiFactory.h"
#include "IO/UI/Transfer/TcpClientTransferUi.h"
#include "IO/UI/Transfer/TcpServerTransferUi.h"
#include "IO/UI/Transfer/UdpClientTransferUi.h"
#include "IO/UI/Transfer/WebSocketClientTransferUi.h"
#include "IO/UI/Transfer/WebSocketServerTransferUi.h"
#include "IO/xIO.h"

#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
#include "IO/IO/Transfer/SerialPortTransfer.h"
#include "IO/UI/Transfer/SerialPortTransferUi.h"
#endif

#include "Common/CRC.h"
#include "CommunicationSettings.h"
#include "InputSettings.h"
#include "OutputSettings.h"
#include "Unit/SyntaxHighlighter.h"

struct ParameterKeys
{
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
    const QString tcpClientTransferItems{"tcpClientTransferItems"};
    const QString tcpServerTransferItems{"tcpServerTransferItems"};
    const QString webSocketClientTransferItems{"webSocketClientTransferItems"};
    const QString webSocketServerTransferItems{"webSocketServerTransferItems"};
} g_keys;

IOPage::IOPage(ControllerDirection direction, QSettings *settings, QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::IOPage}
    , m_io{nullptr}
    , m_ioUi{nullptr}
    , m_ioSettings{nullptr}
    , m_outputSettings{nullptr}
    , m_inputSettings{nullptr}
    , m_writeTimer{new QTimer(this)}
    , m_updateLabelInfoTimer{new QTimer(this)}
    , m_highlighter{new SyntaxHighlighter(this)}
    , m_rxStatistician{new xTools::Statistician(this)}
    , m_txStatistician{new xTools::Statistician(this)}
    , m_preset{new xTools::Preset(this)}
    , m_emitter{new xTools::Emitter(this)}
    , m_responder{new xTools::Responder(this)}
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    , m_serialPortTransfer(new xTools::SerialPortTransfer(this))
    , m_serialPortTransferUi(new xTools::SerialPortTransferUi(this))
#endif
    , m_udpClientTransfer(new xTools::UdpClientTransfer(this))
    , m_udpClientTransferUi(new xTools::UdpClientTransferUi())
    , m_tcpClientTransfer(new xTools::TcpClientTransfer(this))
    , m_tcpClientTransferUi(new xTools::TcpClientTransferUi())
    , m_tcpServerTransfer(new xTools::TcpServerTransfer(this))
    , m_tcpServerTransferUi(new xTools::TcpServerTransferUi())
    , m_webSocketClientTransfer(new xTools::WebSocketClientTransfer(this))
    , m_webSocketClientTransferUi(new xTools::WebSocketClientTransferUi())
    , m_webSocketServerTransfer(new xTools::WebSocketServerTransfer(this))
    , m_webSocketServerTransferUi(new xTools::WebSocketServerTransferUi())
    , m_settings{settings}
{
    if (settings == nullptr) {
        m_settings = new QSettings("IO_Page.ini", QSettings::IniFormat);
    }

    ui->setupUi(this);
    ui->widgetRxInfo->setupIO(m_rxStatistician);
    ui->widgetTxInfo->setupIO(m_txStatistician);

    m_ioList << m_rxStatistician << m_txStatistician << m_preset << m_emitter << m_responder
             << m_udpClientTransfer << m_tcpClientTransfer << m_webSocketClientTransfer
             << m_webSocketServerTransfer;
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    m_ioList << m_serialPortTransfer;
#endif

    if (direction == ControllerDirection::Right) {
        auto *l = qobject_cast<QHBoxLayout *>(layout());
        if (l) {
            auto item = l->takeAt(0);
            l->addItem(item);
        }
    }

    m_writeTimer->setInterval(1000);
    connect(m_writeTimer, &QTimer::timeout, this, &IOPage::writeBytes);

    m_updateLabelInfoTimer->setInterval(100);
    connect(m_updateLabelInfoTimer, &QTimer::timeout, this, &IOPage::updateLabelInfo);
    m_updateLabelInfoTimer->start();

    initUi();

    onShowStatisticianChanged(false);
}

IOPage::~IOPage()
{
    saveControllerParameters();
    delete ui;
}

QVariantMap IOPage::save()
{
    QVariantMap map;
    map.insert(g_keys.communicationType, ui->comboBoxCommunicationTypes->currentData());
    map.insert(g_keys.communicationSettings, m_ioSettings->save());
    if (m_ioUi) {
        map.insert(g_keys.communication, m_ioUi->save());
    }

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
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    map.insert(g_keys.serialPortTransferItems, m_serialPortTransferUi->save());
#endif
    map.insert(g_keys.udpClientTransferItems, m_udpClientTransferUi->save());
    map.insert(g_keys.tcpClientTransferItems, m_tcpClientTransferUi->save());
    map.insert(g_keys.tcpServerTransferItems, m_tcpServerTransferUi->save());
    map.insert(g_keys.webSocketClientTransferItems, m_webSocketClientTransferUi->save());
    map.insert(g_keys.webSocketServerTransferItems, m_webSocketServerTransferUi->save());

    return map;
}

void IOPage::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    int communicationType = parameters.value(g_keys.communicationType).toInt();
    int index = ui->comboBoxCommunicationTypes->findData(communicationType);
    QVariantMap communicationSettings = parameters.value(g_keys.communicationSettings).toMap();
    m_ioSettings->load(communicationSettings);
    ui->comboBoxCommunicationTypes->setCurrentIndex(index == -1 ? 0 : index);
    if (m_ioUi) {
        m_ioUi->load(parameters.value(g_keys.communication).toMap());
    }

    int outputFormat = parameters.value(g_keys.outputFormat).toInt();
    bool outputRx = parameters.value(g_keys.outputRx).toBool();
    bool outputTx = parameters.value(g_keys.outputTx).toBool();
    bool outputFlag = parameters.value(g_keys.outputFlag).toBool();
    bool outputDate = parameters.value(g_keys.outputDate).toBool();
    bool outputTime = parameters.value(g_keys.outputTime).toBool();
    bool outputMs = parameters.value(g_keys.outputMs).toBool();
    QVariantMap outputSettings = parameters.value(g_keys.outputSettings).toMap();

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
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    m_serialPortTransferUi->load(parameters.value(g_keys.serialPortTransferItems).toMap());
#endif
    m_udpClientTransferUi->load(parameters.value(g_keys.udpClientTransferItems).toMap());
    m_tcpClientTransferUi->load(parameters.value(g_keys.tcpClientTransferItems).toMap());
    m_tcpServerTransferUi->load(parameters.value(g_keys.tcpServerTransferItems).toMap());
    m_webSocketClientTransferUi->load(parameters.value(g_keys.webSocketClientTransferItems).toMap());
    m_webSocketServerTransferUi->load(parameters.value(g_keys.webSocketServerTransferItems).toMap());
    // clang-format on
}

void IOPage::initUi()
{
#if 0
    const QIcon icon = QIcon(":/Resources/Icons/IconSettings.svg");
    ui->pushButtonCommunicationSettings->setIcon(icon);
    ui->pushButtonOutputSettings->setIcon(icon);
    ui->pushButtonInputSettings->setIcon(icon);
#endif

    initUiCommunication();
    initUiOutputControl();
    initUiInputControl();
    initUiOutput();
    initUiInput();
}

void IOPage::initUiCommunication()
{
    connect(ui->comboBoxCommunicationTypes,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &IOPage::onCommunicationTypeChanged);
    connect(ui->pushButtonCommunicationOpen,
            &QPushButton::clicked,
            this,
            &IOPage::onOpenButtonClicked);

    QPushButton *target = ui->pushButtonCommunicationSettings;
    m_ioSettings = new CommunicationSettings();
    setupMenu(target, m_ioSettings);

    xTools::xIO::setupCommunicationTypes(ui->comboBoxCommunicationTypes);
}

void IOPage::initUiOutputControl()
{
    xTools::xIO::setupTextFormat(ui->comboBoxOutputFormat);
    ui->checkBoxOutputRx->setChecked(true);
    ui->checkBoxOutputTx->setChecked(true);
    ui->checkBoxOutputTime->setChecked(true);

    m_outputSettings = new OutputSettings();
    setupMenu(ui->pushButtonOutputSettings, m_outputSettings);

    connect(m_outputSettings,
            &OutputSettings::highlighterEnableChanged,
            this,
            &IOPage::onHighlighterEnableChanged);

    connect(m_outputSettings,
            &OutputSettings::highlighterKeywordsChanged,
            this,
            &IOPage::onHighlighterKeywordsChanged);
    connect(m_outputSettings,
            &OutputSettings::showStatisticianChanged,
            this,
            &IOPage::onShowStatisticianChanged);
    m_highlighter->setDocument(ui->textBrowserOutput->document());
}

void IOPage::initUiInputControl()
{
    connect(ui->comboBoxInputFormat,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &IOPage::onInputFormatChanged);
    connect(ui->comboBoxInputInterval,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &IOPage::onCycleIntervalChanged);
    connect(ui->pushButtonInputWriteBytes, &QPushButton::clicked, this, &IOPage::writeBytes);

    xTools::xIO::setupTextFormat(ui->comboBoxInputFormat);
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

void IOPage::initUiOutput()
{
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    ui->tabWidgetTransfers->addTab(m_serialPortTransferUi, tr("Serial Port"));
#endif
    ui->tabWidgetTransfers->addTab(m_udpClientTransferUi, tr("UDP Client"));
    ui->tabWidgetTransfers->addTab(m_tcpClientTransferUi, tr("TCP Client"));
    ui->tabWidgetTransfers->addTab(m_tcpServerTransferUi, tr("TCP Server"));
    ui->tabWidgetTransfers->addTab(m_webSocketClientTransferUi, tr("WebSocket Client"));
    ui->tabWidgetTransfers->addTab(m_webSocketServerTransferUi, tr("WebSocket Server"));

    ui->tabPresets->setupIO(m_preset);
    ui->tabEmitter->setupIO(m_emitter);
    ui->tabResponder->setupIO(m_responder);
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
    m_serialPortTransferUi->setupIO(m_serialPortTransfer);
#endif
    m_udpClientTransferUi->setupIO(m_udpClientTransfer);
    m_tcpClientTransferUi->setupIO(m_tcpClientTransfer);
    m_tcpServerTransferUi->setupIO(m_tcpServerTransfer);
    m_webSocketClientTransferUi->setupIO(m_webSocketClientTransfer);
    m_webSocketServerTransferUi->setupIO(m_webSocketServerTransfer);

    ui->toolButtonInputPreset->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonInputPreset->setMenu(ui->tabPresets->menu());
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidgetTransfers->setCurrentIndex(0);
}

void IOPage::initUiInput()
{
    // Nothing to do
}

void IOPage::onCommunicationTypeChanged()
{
    if (m_ioUi != nullptr) {
        saveControllerParameters();

        m_ioUi->setParent(nullptr);
        m_ioUi->deleteLater();
        m_ioUi = nullptr;
    }

    if (!ui->comboBoxCommunicationTypes->count()) {
        return;
    }

    int type = ui->comboBoxCommunicationTypes->currentData().toInt();
    auto cookedType = static_cast<xTools::xIO::CommunicationType>(type);
    m_ioUi = xTools::IOUiFactory::singleton().createDeviceUi(cookedType);
    if (m_ioUi) {
        loadControllerParameters();
        ui->verticalLayoutCommunicationController->addWidget(m_ioUi);
    }
}

void IOPage::onCycleIntervalChanged()
{
    int interval = ui->comboBoxInputInterval->currentData().toInt();
    if (interval > 0) {
        m_writeTimer->start(interval);
    } else {
        m_writeTimer->stop();
    }
}

void IOPage::onInputFormatChanged()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    xTools::xIO::setupTextFormatValidator(ui->lineEditInput,
                                          static_cast<xTools::xIO::TextFormat>(format));
    ui->lineEditInput->clear();
}

void IOPage::onOpenButtonClicked()
{
    ui->pushButtonCommunicationOpen->setEnabled(false);
    if (m_io) {
        closeCommunication();
    } else {
        openCommunication();
    }
}

void IOPage::onHighlighterEnableChanged()
{
    bool enabled = m_outputSettings->isEnableHighlighter();
    m_highlighter->setEnabled(enabled);
}

void IOPage::onHighlighterKeywordsChanged()
{
    QStringList keywords = m_outputSettings->highlighterKeywords();
    m_highlighter->setKeywords(keywords);
}

void IOPage::onShowStatisticianChanged(bool checked)
{
    ui->labelRx->setVisible(checked);
    ui->labelTx->setVisible(checked);
    ui->lineRxTx->setVisible(checked);
    ui->widgetRxInfo->setVisible(checked);
    ui->widgetTxInfo->setVisible(checked);
}

void IOPage::onOpened()
{
    for (auto io : m_ioList) {
        io->start();
    }

    setUiEnabled(false);
    ui->pushButtonCommunicationOpen->setEnabled(true);
    ui->pushButtonCommunicationOpen->setText(tr("Close"));
    onCycleIntervalChanged();
}

void IOPage::onClosed()
{
    for (auto io : m_ioList) {
        io->exit();
        io->wait();
    }

    m_writeTimer->stop();
    setUiEnabled(true);
    ui->pushButtonCommunicationOpen->setEnabled(true);
    ui->pushButtonCommunicationOpen->setText(tr("Open"));
}

void IOPage::onErrorOccurred(const QString &error)
{
    closeCommunication();
    if (!error.isEmpty()) {
        QMessageBox::warning(this, tr("Error Occurred"), error);
    }
}

void IOPage::onWarningOccurred(const QString &warning)
{
    QMessageBox::warning(this, tr("Warning"), warning);
}

void IOPage::onBytesRead(const QByteArray &bytes, const QString &from)
{
    m_ioSettings->saveData(bytes, false);
    m_rxStatistician->inputBytes(bytes);
    outputText(bytes, from, true);
}

void IOPage::onBytesWritten(const QByteArray &bytes, const QString &to)
{
    m_ioSettings->saveData(bytes, true);
    m_txStatistician->inputBytes(bytes);
    outputText(bytes, to, false);
}

void IOPage::openCommunication()
{
    int type = ui->comboBoxCommunicationTypes->currentData().toInt();
    m_io = xTools::IOFactory::singleton().createDevice(type);
    if (m_io) {
        setUiEnabled(false);

        // clang-format off
        connect(m_io, &xTools::Communication::opened, this, &IOPage::onOpened);
        connect(m_io, &xTools::Communication::closed, this, &IOPage::onClosed);
        connect(m_io, &xTools::Communication::bytesWritten, this, &IOPage::onBytesWritten);
        connect(m_io, &xTools::Communication::bytesRead, this, &IOPage::onBytesRead);
        connect(m_io, &xTools::Communication::errorOccurred, this, &IOPage::onErrorOccurred);
        connect(m_io, &xTools::Communication::warningOccurred, this, &::IOPage::onWarningOccurred);
        connect(m_io, &xTools::Communication::outputBytes, m_responder, &xTools::Responder::inputBytes);
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
        connect(m_io, &xTools::Communication::outputBytes, m_serialPortTransfer, &xTools::SerialPortTransfer::inputBytes);
#endif
        connect(m_io, &xTools::Communication::outputBytes, m_udpClientTransfer, &xTools::UdpClientTransfer::inputBytes);
        connect(m_io, &xTools::Communication::outputBytes, m_tcpClientTransfer, &xTools::TcpClientTransfer::inputBytes);
        connect(m_io, &xTools::Communication::outputBytes, m_tcpServerTransfer, &xTools::TcpServerTransfer::inputBytes);
        connect(m_io, &xTools::Communication::outputBytes, m_webSocketClientTransfer, &xTools::WebSocketClientTransfer::inputBytes);
        connect(m_io, &xTools::Communication::outputBytes, m_webSocketServerTransfer, &xTools::WebSocketServerTransfer::inputBytes);

        connect(m_preset, &xTools::Preset::outputBytes, m_io, &xTools::Communication::inputBytes);
        connect(m_emitter, &xTools::Preset::outputBytes, m_io, &xTools::Communication::inputBytes);
        connect(m_responder, &xTools::Responder::outputBytes, m_io, &xTools::Communication::inputBytes);
#ifdef X_TOOLS_ENABLE_MODULE_SERIAL_PORT
        connect(m_serialPortTransfer, &xTools::SerialPortTransfer::outputBytes, m_io, &xTools::Communication::inputBytes);
#endif
        connect(m_udpClientTransfer, &xTools::UdpClientTransfer::outputBytes, m_io, &xTools::Communication::inputBytes);
        connect(m_tcpClientTransfer, &xTools::TcpClientTransfer::outputBytes, m_io, &xTools::Communication::inputBytes);
        connect(m_tcpServerTransfer, &xTools::TcpServerTransfer::outputBytes, m_io, &xTools::Communication::inputBytes);
        connect(m_webSocketClientTransfer, &xTools::WebSocketClientTransfer::outputBytes, m_io, &xTools::Communication::inputBytes);
        connect(m_webSocketServerTransfer, &xTools::WebSocketServerTransfer::outputBytes, m_io, &xTools::Communication::inputBytes);
        // clang-format on

        QVariantMap parameters = m_ioUi->save();
        m_ioUi->setupDevice(m_io);
        m_io->load(parameters);
        m_io->setParameters(parameters);
        m_io->openDevice();
    }
}

void IOPage::closeCommunication()
{
    if (m_io) {
        m_io->exit();
        m_io->wait();
        m_io->deleteLater();
        m_io = nullptr;
    }

    if (m_ioUi) {
        m_ioUi->setupDevice(nullptr);
    }
}

void IOPage::writeBytes()
{
    if (!m_io) {
        return;
    }

    auto parameters = m_inputSettings->parameters();
    QByteArray prefix = xTools::xIO::cookedAffixes(
        static_cast<xTools::xIO::Affixes>(parameters.prefix));
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = xTools::xIO::cookedAffixes(
        static_cast<xTools::xIO::Affixes>(parameters.suffix));

    if (parameters.appendCrc) {
        m_io->inputBytes(prefix + payload + crc + suffix);
    } else {
        m_io->inputBytes(prefix + payload + suffix);
    }
}

void IOPage::updateLabelInfo()
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();

    QByteArray prefix = xTools::xIO::cookedAffixes(
        static_cast<xTools::xIO::Affixes>(parameters.prefix));
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = xTools::xIO::cookedAffixes(
        static_cast<xTools::xIO::Affixes>(parameters.suffix));

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

void IOPage::setupMenu(QPushButton *target, QWidget *actionWidget)
{
    QMenu *menu = new QMenu(target);
    QWidgetAction *action = new QWidgetAction(menu);
    action->setDefaultWidget(actionWidget);
    menu->addAction(action);
    target->setMenu(menu);
}

void IOPage::setUiEnabled(bool enabled)
{
    if (m_ioUi) {
        m_ioUi->setUiEnabled(enabled);
    }

    ui->comboBoxCommunicationTypes->setEnabled(enabled);
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

void IOPage::outputText(const QByteArray &bytes, const QString &flag, bool isRx)
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
    QString text = xTools::xIO::bytes2string(bytes, static_cast<xTools::xIO::TextFormat>(format));
    QString rxtx = isRx ? QStringLiteral("Rx") : QStringLiteral("Tx");
    rxtx = QString("<font color=%1>%2</font>").arg(isRx ? "blue" : "green", rxtx);

    QString header;
    if (showFlag) {
        header = QString("%1 %2 %3").arg(rxtx, dateTimeString, flag);
    } else {
        header = QString("%1 %2").arg(rxtx, dateTimeString);
    }

    header = header.trimmed();
    header = QString("<font color=silver>[%1]</font>").arg(header);
    QString outputText = QString("%1 %2").arg(header, text);
    if (m_outputSettings->isEnableFilter()) {
        QString filter = m_outputSettings->filterText();
        if (outputText.contains(filter)) {
            ui->textBrowserOutput->append(outputText);
        }
    } else {
        ui->textBrowserOutput->append(outputText);
    }
}

void IOPage::saveControllerParameters()
{
    if (m_ioUi != nullptr) {
        auto parameters = m_ioUi->save();
        int type = static_cast<int>(m_ioUi->type());
        m_settings->setValue(QString("controller_%1").arg(type), parameters);
    }
}

void IOPage::loadControllerParameters()
{
    int type = ui->comboBoxCommunicationTypes->currentData().toInt();
    auto parameters = m_settings->value(QString("controller_%1").arg(type));
    if (!parameters.isNull()) {
        m_ioUi->load(parameters.toMap());
    }
}

QByteArray IOPage::payload() const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    QString text = ui->lineEditInput->text();
    int format = ui->comboBoxInputFormat->currentData().toInt();
    auto escapeCharacter = static_cast<xTools::xIO::EscapeCharacter>(parameters.escapeCharacter);
    text = xTools::xIO::cookedEscapeCharacter(text, escapeCharacter);
    auto cookedFormat = static_cast<xTools::xIO::TextFormat>(format);
    QByteArray payload = xTools::xIO::string2bytes(text, cookedFormat);
    return payload;
}

QByteArray IOPage::crc(const QByteArray &payload) const
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
