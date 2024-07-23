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

#include "CommunicationSettings.h"
#include "IO/IO/Communication/Communication.h"
#include "IO/IO/IOFactory.h"
#include "IO/UI/Communication/CommunicationUi.h"
#include "IO/UI/IOUiFactory.h"
#include "IO/xIO.h"
#include "InputSettings.h"
#include "OutputSettings.h"
#include "Unit/SyntaxHighlighter.h"

IOPage::IOPage(QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::IOPage}
    , m_io{nullptr}
    , m_ioUi{nullptr}
    , m_ioSettings{nullptr}
    , m_outputSettings{nullptr}
    , m_inputSettings{nullptr}
    , m_writeTimer{new QTimer(this)}
    , m_updateLabelnfoTimer{new QTimer(this)}
    , m_highlighter{new SyntaxHighlighter(this)}
{
    ui->setupUi(this);

    m_writeTimer->setInterval(1000);
    connect(m_writeTimer, &QTimer::timeout, this, &IOPage::writeBytes);

    m_updateLabelnfoTimer->setInterval(100);
    connect(m_updateLabelnfoTimer, &QTimer::timeout, this, &IOPage::updateLabelInfo);
    m_updateLabelnfoTimer->start();

    initUi();
}

IOPage::~IOPage()
{
    delete ui;
}

QVariantMap IOPage::save()
{
    QVariantMap map;
    map.insert(m_keys.communicationType, ui->comboBoxCommmunicationTypes->currentData());
    map.insert(m_keys.communicationSettings, m_ioSettings->save());

    map.insert(m_keys.outputFormat, ui->comboBoxOutputFormat->currentData());
    map.insert(m_keys.outputRx, ui->checkBoxOutputRx->isChecked());
    map.insert(m_keys.outputTx, ui->checkBoxOutputTx->isChecked());
    map.insert(m_keys.outputFlag, ui->checkBoxOutputFlag->isChecked());
    map.insert(m_keys.outputDate, ui->checkBoxOutputDate->isChecked());
    map.insert(m_keys.outputTime, ui->checkBoxOutputTime->isChecked());
    map.insert(m_keys.outputMs, ui->checkBoxOutputMs->isChecked());
    map.insert(m_keys.outputSettings, m_outputSettings->save());

    map.insert(m_keys.cycleInterval, ui->comboBoxInputInterval->currentData());
    map.insert(m_keys.inputFormat, ui->comboBoxInputFormat->currentData());
    map.insert(m_keys.inputSettings, m_inputSettings->save());

    return map;
}

void IOPage::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    int communicationType = parameters.value(m_keys.communicationType).toInt();
    QVariantMap communicationSettings = parameters.value(m_keys.communicationSettings).toMap();

    int outputFormat = parameters.value(m_keys.outputFormat).toInt();
    bool outputRx = parameters.value(m_keys.outputRx).toBool();
    bool outputTx = parameters.value(m_keys.outputTx).toBool();
    bool outputFlag = parameters.value(m_keys.outputFlag).toBool();
    bool outputDate = parameters.value(m_keys.outputDate).toBool();
    bool outputTime = parameters.value(m_keys.outputTime).toBool();
    bool outputMs = parameters.value(m_keys.outputMs).toBool();
    QVariantMap outputSettings = parameters.value(m_keys.outputSettings).toMap();

    int inputInterval = parameters.value(m_keys.cycleInterval).toInt();
    int inputFormat = parameters.value(m_keys.inputFormat).toInt();
    QVariantMap inputSettings = parameters.value(m_keys.inputSettings).toMap();

    int index = ui->comboBoxCommmunicationTypes->findData(communicationType);
    ui->comboBoxCommmunicationTypes->setCurrentIndex(index == -1 ? 0 : index);

    index = ui->comboBoxOutputFormat->findData(outputFormat);
    ui->comboBoxCommmunicationTypes->setCurrentIndex(index == -1 ? 0 : index);
    m_ioSettings->load(communicationSettings);

    index = ui->comboBoxOutputFormat->findData(outputFormat);
    ui->comboBoxOutputFormat->setCurrentIndex(index == -1 ? 0 : index);
    ui->checkBoxOutputRx->setChecked(outputRx);
    ui->checkBoxOutputTx->setChecked(outputTx);
    ui->checkBoxOutputFlag->setChecked(outputFlag);
    ui->checkBoxOutputDate->setChecked(outputDate);
    ui->checkBoxOutputTime->setChecked(outputTime);
    ui->checkBoxOutputMs->setChecked(outputMs);
    m_outputSettings->load(outputSettings);

    index = ui->comboBoxInputInterval->findData(inputInterval);
    ui->comboBoxInputInterval->setCurrentIndex(index == -1 ? 0 : index);
    index = ui->comboBoxInputFormat->findData(inputFormat);
    ui->comboBoxInputFormat->setCurrentIndex(index == -1 ? 0 : index);
    m_inputSettings->load(inputSettings);
}

void IOPage::initUi()
{
    initUiCommunication();
    initUiOutputControl();
    initUiInputControl();
    initUiOutput();
    initUiInput();
}

void IOPage::initUiCommunication()
{
    connect(ui->comboBoxCommmunicationTypes,
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

    xIO::setupCommunicationTypes(ui->comboBoxCommmunicationTypes);
}

void IOPage::initUiOutputControl()
{
    xIO::setupTextFormat(ui->comboBoxOutputFormat);
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

    xIO::setupTextFormat(ui->comboBoxInputFormat);
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
    // Nothing to do
}

void IOPage::initUiInput()
{
    // Nothing to do
}

void IOPage::onCommunicationTypeChanged()
{
    if (m_ioUi != nullptr) {
        m_ioUi->setParent(nullptr);
        m_ioUi->deleteLater();
        m_ioUi = nullptr;
    }

    if (!ui->comboBoxCommmunicationTypes->count()) {
        return;
    }

    int type = ui->comboBoxCommmunicationTypes->currentData().toInt();
    auto cookedType = static_cast<xIO::CommunicationType>(type);
    m_ioUi = IOUiFactory::singleton().createDeviceUi(cookedType);
    if (m_ioUi) {
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
    xIO::setupTextFormatValidator(ui->lineEditInput, static_cast<xIO::TextFormat>(format));
    ui->lineEditInput->clear();
}

void IOPage::onOpenButtonClicked()
{
    ui->pushButtonCommunicationOpen->setEnabled(false);
    if (m_io) {
        close();
    } else {
        open();
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

void IOPage::onOpened()
{
    setUiEnabled(false);
    ui->pushButtonCommunicationOpen->setEnabled(true);
    ui->pushButtonCommunicationOpen->setText(tr("Close"));
    onCycleIntervalChanged();
}

void IOPage::onClosed()
{
    m_writeTimer->stop();
    setUiEnabled(true);
    ui->pushButtonCommunicationOpen->setEnabled(true);
    ui->pushButtonCommunicationOpen->setText(tr("Open"));
}

void IOPage::onErrorOccurred(const QString &error)
{
    close();
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
    outputText(bytes, from, true);
}

void IOPage::onBytesWritten(const QByteArray &bytes, const QString &to)
{
    m_ioSettings->saveData(bytes, true);
    outputText(bytes, to, false);
}

void IOPage::open()
{
    int type = ui->comboBoxCommmunicationTypes->currentData().toInt();
    m_io = IOFactory::singleton().createDevice(type);
    if (m_io) {
        setUiEnabled(false);

        connect(m_io, &Communication::opened, this, &IOPage::onOpened);
        connect(m_io, &Communication::closed, this, &IOPage::onClosed);
        connect(m_io, &Communication::bytesWritten, this, &IOPage::onBytesWritten);
        connect(m_io, &Communication::bytesRead, this, &IOPage::onBytesRead);
        connect(m_io, &Communication::errorOccurred, this, &IOPage::onErrorOccurred);
        connect(m_io, &Communication::warningOccurred, this, &::IOPage::onWarningOccurred);

        QVariantMap parameters = m_ioUi->save();
        m_ioUi->setupDevice(m_io);
        m_io->load(parameters);
        m_io->setParameters(parameters);
        m_io->openDevice();
    }
}

void IOPage::close()
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
    QByteArray prefix = xIO::cookedAffixes(static_cast<xIO::Affixes>(parameters.prefix));
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = xIO::cookedAffixes(static_cast<xIO::Affixes>(parameters.suffix));

    if (parameters.appendCrc) {
        m_io->inputBytes(prefix + payload + crc + suffix);
    } else {
        m_io->inputBytes(prefix + payload + suffix);
    }
}

void IOPage::updateLabelInfo()
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();

    QByteArray prefix = xIO::cookedAffixes(static_cast<xIO::Affixes>(parameters.prefix));
    QByteArray payload = this->payload();
    QByteArray crc = this->crc(payload);
    QByteArray suffix = xIO::cookedAffixes(static_cast<xIO::Affixes>(parameters.suffix));

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

    ui->comboBoxCommmunicationTypes->setEnabled(enabled);
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
    QString text = xIO::bytes2string(bytes, static_cast<xIO::TextFormat>(format));
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

QByteArray IOPage::payload() const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    QString text = ui->lineEditInput->text();
    int format = ui->comboBoxInputFormat->currentData().toInt();
    auto escapeCharacter = static_cast<xIO::EscapeCharacter>(parameters.escapeCharacter);
    text = xIO::cookedEscapeCharacter(text, escapeCharacter);
    QByteArray payload = xIO::string2bytes(text, static_cast<xIO::TextFormat>(format));
    return payload;
}

QByteArray IOPage::crc(const QByteArray &payload) const
{
    InputSettings::Parameters parameters = m_inputSettings->parameters();
    return xIO::calculateCrc(payload,
                             static_cast<xIO::CrcAlgorithm>(parameters.algorithm),
                             parameters.startIndex,
                             parameters.endIndex,
                             parameters.bigEndian);
}
