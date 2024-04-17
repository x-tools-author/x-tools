/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCanBusStudioUi.h"
#include "ui_xToolsCanBusStudioUi.h"

#include <QCanBus>
#include <QCheckBox>
#include <QDateTime>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QVector>

#include "xToolsSettings.h"

xToolsCanBusStudioUi::xToolsCanBusStudioUi(QWidget* parent)
    : QWidget{parent}
    , ui(new Ui::xToolsCanBusStudioUi)
    , m_device(Q_NULLPTR)
{
    ui->setupUi(this);
    if (!m_settings) {
        m_settings = xToolsSettings::instance();
    }

    initUi();
    initSetting();

    // Device is not connected.
    updateUiState(false);
}

xToolsCanBusStudioUi::~xToolsCanBusStudioUi()
{
    delete ui;
}

void xToolsCanBusStudioUi::initUi()
{
    initUiSelectPlugin();
    initUiSpecifyConfiguration();
    initUiCanFrame();
    initUiSendCanFrame();
}

void xToolsCanBusStudioUi::initUiSelectPlugin()
{
    ui->pluginComboBox->clear();
    ui->pluginComboBox->addItems(QCanBus::instance()->plugins());
    ui->disconnectPushButton->setEnabled(false);
    ui->connectPushButton->setEnabled(true);

    connect(ui->pluginComboBox,
            SIGNAL(currentTextChanged(QString)),
            this,
            SLOT(onPluginChanged(QString)));
    connect(ui->disconnectPushButton, SIGNAL(clicked(bool)), this, SLOT(onDisconnectClicked()));
    connect(ui->connectPushButton, SIGNAL(clicked(bool)), this, SLOT(onConnectClicked()));
}

void xToolsCanBusStudioUi::initUiSpecifyConfiguration()
{
    setOptions(ui->loopbackComboBox, true);
    setOptions(ui->receivOwnComboBox, true);
    setOptions(ui->canFdComboBox, false);
    setBitRates(ui->bitrateComboBox, false);
    setBitRates(ui->dataBitrateComboBox, true);

    ui->interfaceNameComboBox->lineEdit()->setPlaceholderText(tr("can0"));

    connect(ui->customConfigurationCheckBox,
            &QCheckBox::clicked,
            this,
            &xToolsCanBusStudioUi::onCustomConfigurationChanged);
    connect(ui->loopbackComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onLoopbackIndexChanged);
    connect(ui->receivOwnComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onReceiveOwnIndexChanged);
    connect(ui->canFdComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onCanFdIndexChanged);
    connect(ui->bitrateComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onBitrateChanged);
    connect(ui->dataBitrateComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onDataBitrateChanged);
}

void xToolsCanBusStudioUi::initUiCanFrame()
{
    ui->frameTypeComboBox->clear();
    ui->frameTypeComboBox->addItem(tr("DataFrame"), QCanBusFrame::DataFrame);
    ui->frameTypeComboBox->addItem(tr("ErrorFrame"), QCanBusFrame::ErrorFrame);
    ui->frameTypeComboBox->addItem(tr("RemoteRequestFrame"), QCanBusFrame::RemoteRequestFrame);

    connect(ui->frameTypeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onFrameTypeChanged);
    connect(ui->extendedFormatCheckBox,
            &QCheckBox::clicked,
            this,
            &xToolsCanBusStudioUi::onExtendedFormatChanged);
    connect(ui->flexibleDataRateCheckBox,
            &QCheckBox::clicked,
            this,
            &xToolsCanBusStudioUi::onFlexibleDataRateChanged);
    connect(ui->bitrateSwitchCheckBox,
            &QCheckBox::clicked,
            this,
            &xToolsCanBusStudioUi::onBitrateSwitchChanged);
}

void xToolsCanBusStudioUi::initUiSendCanFrame()
{
    const QString inputTips = tr("Hex");
    ui->frameIdComboBox->lineEdit()->setPlaceholderText(inputTips);
    ui->payloadComboBox->lineEdit()->setPlaceholderText(inputTips);

    connect(ui->sendPushButton,
            &QPushButton::clicked,
            this,
            &xToolsCanBusStudioUi::onSendButtonClicked);
}

void xToolsCanBusStudioUi::initSetting()
{
    initSettingSelectPlugin();
    initSettingSpecifyConfiguration();
    initSettingCanFrame();
    initSettingSendCanFrame();
}

void xToolsCanBusStudioUi::initSettingSelectPlugin()
{
    setCurrentIndex(ui->pluginComboBox, m_settingKeyCtx.pluginIndex);
}

void xToolsCanBusStudioUi::initSettingSpecifyConfiguration()
{
    QString name = m_settings->value(m_settingKeyCtx.interfaceName).toString();
    ui->interfaceNameComboBox->lineEdit()->setText(name);

    setChecked(ui->customConfigurationCheckBox, m_settingKeyCtx.customConfiguration);
    setCurrentIndex(ui->loopbackComboBox, m_settingKeyCtx.loopback);
    setCurrentIndex(ui->receivOwnComboBox, m_settingKeyCtx.receiveOwn);
    setCurrentIndex(ui->bitrateComboBox, m_settingKeyCtx.bitrate);
    setCurrentIndex(ui->canFdComboBox, m_settingKeyCtx.canFd);
    setCurrentIndex(ui->dataBitrateComboBox, m_settingKeyCtx.dataBitRate);

    bool enable = m_settings->value(m_settingKeyCtx.customConfiguration).toBool();
    setCustomConfigurationEnable(enable);
}

void xToolsCanBusStudioUi::initSettingCanFrame()
{
    setCurrentIndex(ui->frameTypeComboBox, m_settingKeyCtx.frameTypeIndex);
    setChecked(ui->extendedFormatCheckBox, m_settingKeyCtx.extendedFormat);
    setChecked(ui->flexibleDataRateCheckBox, m_settingKeyCtx.flexibleDataRate);
    setChecked(ui->bitrateSwitchCheckBox, m_settingKeyCtx.bitrateSwitch);

    onFrameTypeChanged();
}

void xToolsCanBusStudioUi::initSettingSendCanFrame() {}

void xToolsCanBusStudioUi::onPluginChanged(QString plugin)
{
    ui->interfaceNameComboBox->clear();
    m_interfaces = QCanBus::instance()->availableDevices(plugin);
    for (const QCanBusDeviceInfo& info : qAsConst(m_interfaces)) {
        ui->interfaceNameComboBox->addItem(info.name());
    }

    m_settings->setValue(m_settingKeyCtx.pluginIndex, ui->pluginComboBox->findText(plugin));
}

void xToolsCanBusStudioUi::onDisconnectClicked()
{
    if (m_device) {
        m_device->disconnectDevice();
        m_device->deleteLater();
        m_device = Q_NULLPTR;
    }

    updateUiState(false);
}

void xToolsCanBusStudioUi::onConnectClicked()
{
    const QString pluginName = ui->pluginComboBox->currentText();
    const QString interfaceName = ui->interfaceNameComboBox->currentText();

    if (interfaceName.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Interface Name is Empty"),
                             tr("Interface name is empty, "
                                "please input the name then try again!"));
        return;
    }

    QString errorString;
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    m_device = QCanBus::instance()->createDevice(pluginName, interfaceName, &errorString);
#else
    m_device = QCanBus::instance()->createDevice(pluginName.toLatin1(), interfaceName);
    errorString = tr("Create device failed!");
#endif
    if (!m_device) {
        QMessageBox::warning(this, tr("Error Occurred"), errorString);
        return;
    }

    connect(m_device, &QCanBusDevice::errorOccurred, this, &xToolsCanBusStudioUi::onErrorOccure);
    connect(m_device, &QCanBusDevice::framesReceived, this, &xToolsCanBusStudioUi::onFrameReceived);
    connect(m_device, &QCanBusDevice::framesWritten, this, &xToolsCanBusStudioUi::onFrameWritten);

    auto items = configurationItems();
    for (auto& item : items) {
        m_device->setConfigurationParameter(item.first, item.second);
    }

    if (!m_device->connectDevice()) {
        QMessageBox::warning(this,
                             tr("Connection Error"),
                             tr("Connection error: %1").arg(m_device->errorString()));
        m_device->deleteLater();
        m_device = Q_NULLPTR;
        return;
    }

    m_settings->setValue(m_settingKeyCtx.interfaceName, interfaceName);
    updateUiState(true);
}

void xToolsCanBusStudioUi::onLoopbackIndexChanged(int index)
{
    m_settings->setValue(m_settingKeyCtx.loopback, index);
}

void xToolsCanBusStudioUi::onCustomConfigurationChanged()
{
    bool checked = ui->customConfigurationCheckBox->isChecked();
    setCustomConfigurationEnable(checked);
    m_settings->setValue(m_settingKeyCtx.customConfiguration, checked);
}

void xToolsCanBusStudioUi::onReceiveOwnIndexChanged(int index)
{
    m_settings->setValue(m_settingKeyCtx.receiveOwn, index);
}

void xToolsCanBusStudioUi::onCanFdIndexChanged(int index)
{
    m_settings->setValue(m_settingKeyCtx.canFd, index);
}

void xToolsCanBusStudioUi::onBitrateChanged(int index)
{
    m_settings->setValue(m_settingKeyCtx.bitrate, index);
}

void xToolsCanBusStudioUi::onDataBitrateChanged(int index)
{
    m_settings->setValue(m_settingKeyCtx.dataBitRate, index);
}

void xToolsCanBusStudioUi::onFrameTypeChanged()
{
    int index = ui->frameTypeComboBox->currentIndex();
    m_settings->setValue(m_settingKeyCtx.frameTypeIndex, index);

    int type = ui->frameTypeComboBox->currentData().toInt();
    if (type == QCanBusFrame::DataFrame) {
        ui->flexibleDataRateCheckBox->setEnabled(true);
        bool checked = ui->flexibleDataRateCheckBox->isChecked();
        ui->bitrateSwitchCheckBox->setEnabled(checked);
    } else {
        ui->flexibleDataRateCheckBox->setEnabled(false);
        ui->bitrateSwitchCheckBox->setEnabled(false);
    }
}

void xToolsCanBusStudioUi::onExtendedFormatChanged()
{
    bool checked = ui->extendedFormatCheckBox->isChecked();
    m_settings->setValue(m_settingKeyCtx.extendedFormat, checked);
}

void xToolsCanBusStudioUi::onFlexibleDataRateChanged()
{
    bool checked = ui->flexibleDataRateCheckBox->isChecked();
    m_settings->setValue(m_settingKeyCtx.flexibleDataRate, checked);

    ui->bitrateSwitchCheckBox->setEnabled(checked);
}

void xToolsCanBusStudioUi::onBitrateSwitchChanged()
{
    bool checked = ui->bitrateSwitchCheckBox->isChecked();
    m_settings->setValue(m_settingKeyCtx.bitrateSwitch, checked);
}

void xToolsCanBusStudioUi::onSendButtonClicked()
{
    if (!m_device) {
        QString title = tr("Device is Not Ready");
        QString msg = tr("Device is not ready, please connect the device then try angin!");
        QMessageBox::warning(this, title, msg);
        return;
    }

    const uint frameId = ui->frameIdComboBox->currentText().toUInt(Q_NULLPTR, 16);
    QString data = ui->payloadComboBox->currentText().trimmed();
    const QByteArray payload = QByteArray::fromHex(data.remove(QLatin1Char(' ')).toLatin1());

    QCanBusFrame frame = QCanBusFrame(frameId, payload);
    frame.setExtendedFrameFormat(ui->extendedFormatCheckBox->isChecked());

    if (ui->flexibleDataRateCheckBox->isEnabled()) {
        frame.setFlexibleDataRateFormat(ui->flexibleDataRateCheckBox->isChecked());
    }

    if (ui->bitrateSwitchCheckBox->isEnabled()) {
        frame.setBitrateSwitch(ui->bitrateSwitchCheckBox);
    }

    if (!m_device->writeFrame(frame)) {
        qWarning() << m_device->errorString();
    }

    bool isErrorFrame = frame.frameType() == QCanBusFrame::ErrorFrame;
    QString view = isErrorFrame ? m_device->interpretErrorFrame(frame) : frame.toString();
    if (frame.frameType() == QCanBusFrame::ErrorFrame) {
        view = m_device->interpretErrorFrame(frame);
    } else {
        view = frame.toString();
    }

    QString flag = QString("<font color=green>[Tx] </font>");
    outputMessage(flag + view);
}

void xToolsCanBusStudioUi::onErrorOccure(QCanBusDevice::CanBusError error)
{
    if (m_device) {
        Q_UNUSED(error);
        QMessageBox::warning(this, tr("Error Occure"), m_device->errorString());
        onDisconnectClicked();
    }
}

void xToolsCanBusStudioUi::onFrameReceived()
{
    if (!m_device) {
        return;
    }

    while (m_device->framesAvailable()) {
        const QCanBusFrame frame = m_device->readFrame();

        QString view;
        if (frame.frameType() == QCanBusFrame::ErrorFrame) {
            view = m_device->interpretErrorFrame(frame);
        } else {
            view = frame.toString();
        }

        QString flag = QString("<font color=blue>[Rx] </font>");
        outputMessage(flag + view);
    }
}

void xToolsCanBusStudioUi::onFrameWritten(qint64 framesCount)
{
    qInfo() << framesCount;
}

void xToolsCanBusStudioUi::setOptions(QComboBox* cb, bool usingUnspecified)
{
    if (cb) {
        cb->clear();
        if (usingUnspecified) {
            cb->addItem(tr("unspecified"), QVariant());
        }
        cb->addItem(tr("false"), QVariant(false));
        cb->addItem(tr("true"), QVariant(true));
    }
}

void xToolsCanBusStudioUi::setCurrentIndex(QComboBox* cb, const QString& key)
{
    int index = m_settings->value(key).toInt();
    if (index >= 0 && index <= cb->count() - 1) {
        cb->setCurrentIndex(index);
    }
}

void xToolsCanBusStudioUi::setChecked(QCheckBox* cb, const QString& key)
{
    if (cb) {
        bool checked = m_settings->value(key).toBool();
        cb->setChecked(checked);
    }
}

void xToolsCanBusStudioUi::setCustomConfigurationEnable(bool enable)
{
    ui->errorFilterComboBox->setEnabled(enable);
    ui->loopbackComboBox->setEnabled(enable);
    ui->receivOwnComboBox->setEnabled(enable);
    ui->canFdComboBox->setEnabled(enable);
    ui->bitrateComboBox->setEnabled(enable);
    ui->dataBitrateComboBox->setEnabled(enable);
}

void xToolsCanBusStudioUi::outputMessage(const QString& msg)
{
    QString datetimeString = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString cookedMsg = QString("<font color=silver>%1 </font>").arg(datetimeString);
    ui->textBrowser->append(cookedMsg + msg);
}

void xToolsCanBusStudioUi::updateUiState(bool connected)
{
    ui->connectPushButton->setEnabled(!connected);
    ui->disconnectPushButton->setEnabled(connected);

    ui->interfaceNameComboBox->setEnabled(!connected);
    ui->customConfigurationCheckBox->setEnabled(!connected);
    if (connected) {
        setCustomConfigurationEnable(false);
    } else {
        bool checked = ui->customConfigurationCheckBox->isChecked();
        setCustomConfigurationEnable(checked);
    }
}

QVector<xToolsCanBusStudioUi::ConfigurationItem> xToolsCanBusStudioUi::configurationItems()
{
    QVector<xToolsCanBusStudioUi::ConfigurationItem> items;
    ConfigurationItem item;

    QString errorFilter = ui->errorFilterComboBox->currentText();
    if (!errorFilter.isEmpty()) {
        bool ok = false;
        int dec = errorFilter.toInt(&ok);
        if (ok) {
            item.first = QCanBusDevice::ErrorFilterKey;
            item.second = QVariant::fromValue(QCanBusFrame::FrameErrors(dec));
            items.append(item);
        }
    }

    item.first = QCanBusDevice::LoopbackKey;
    item.second = ui->loopbackComboBox->currentData();
    items.append(item);

    item.first = QCanBusDevice::ReceiveOwnKey;
    item.second = ui->receivOwnComboBox->currentData();
    items.append(item);

    item.first = QCanBusDevice::CanFdKey;
    item.second = ui->canFdComboBox->currentData();
    items.append(item);

    item.first = QCanBusDevice::BitRateKey;
    item.second = ui->bitrateComboBox->currentData();
    items.append(item);

    item.first = QCanBusDevice::DataBitRateKey;
    item.second = ui->dataBitrateComboBox->currentData();
    items.append(item);

    return items;
}

void xToolsCanBusStudioUi::setBitRates(QComboBox* cb, bool isFlexibleDataRateEnable)
{
    if (!cb) {
        return;
    }

    const QVector<int> rates = {10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000};

    const QVector<int> dataRates = {2000000, 4000000, 8000000};

    cb->clear();

    for (int rate : rates) {
        cb->addItem(QString::number(rate), rate);
    }

    if (isFlexibleDataRateEnable) {
        for (int rate : dataRates) {
            cb->addItem(QString::number(rate), rate);
        }
    }
}
