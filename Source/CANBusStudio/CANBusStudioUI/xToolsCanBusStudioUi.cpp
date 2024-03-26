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
#include <QLineEdit>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QVector>

#include "xToolsSettings.h"

const QLoggingCategory gLC("sak.canstudio");

xToolsCanBusStudioUi::xToolsCanBusStudioUi(QWidget* parent)
    : QWidget{parent}
    , ui(new Ui::xToolsCanBusStudioUi)
    , mDevice(Q_NULLPTR)
{
    if (!mSettings) {
        mSettings = xToolsSettings::instance();
    }

    ui->setupUi(this);
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
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCanBusStudioUi::onPluginChanged);
    connect(ui->disconnectPushButton,
            &QPushButton::clicked,
            this,
            &xToolsCanBusStudioUi::onDisconnectClicked);
    connect(ui->connectPushButton, &QPushButton::clicked, this, &xToolsCanBusStudioUi::onConnectClicked);
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
    connect(ui->customBitrateCheckBox,
            &QCheckBox::clicked,
            this,
            &xToolsCanBusStudioUi::onCustomBitrateChanged);
    connect(ui->customDataBitrateCheckBox,
            &QCheckBox::clicked,
            this,
            &xToolsCanBusStudioUi::onCustomDataBitrateChanged);
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

    connect(ui->sendPushButton, &QPushButton::clicked, this, &xToolsCanBusStudioUi::onSendButtonClicked);
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
    setCurrentIndex(ui->pluginComboBox, mSettingKeyCtx.pluginIndex);
}

void xToolsCanBusStudioUi::initSettingSpecifyConfiguration()
{
    QString name = mSettings->value(mSettingKeyCtx.interfaceName).toString();
    ui->interfaceNameComboBox->lineEdit()->setText(name);

    setChecked(ui->customConfigurationCheckBox, mSettingKeyCtx.customConfiguration);
    setCurrentIndex(ui->loopbackComboBox, mSettingKeyCtx.loopback);
    setCurrentIndex(ui->receivOwnComboBox, mSettingKeyCtx.receiveOwn);
    setCurrentIndex(ui->bitrateComboBox, mSettingKeyCtx.bitrate);
    setCurrentIndex(ui->canFdComboBox, mSettingKeyCtx.canFd);
    setCurrentIndex(ui->dataBitrateComboBox, mSettingKeyCtx.dataBitRate);
    setChecked(ui->customBitrateCheckBox, mSettingKeyCtx.customBitRate);
    setChecked(ui->customDataBitrateCheckBox, mSettingKeyCtx.customDataBitRate);

    bool enable = mSettings->value(mSettingKeyCtx.customConfiguration).toBool();
    setCustomConfigurationEnable(enable);
}

void xToolsCanBusStudioUi::initSettingCanFrame()
{
    setCurrentIndex(ui->frameTypeComboBox, mSettingKeyCtx.frameTypeIndex);
    setChecked(ui->extendedFormatCheckBox, mSettingKeyCtx.extendedFormat);
    setChecked(ui->flexibleDataRateCheckBox, mSettingKeyCtx.flexibleDataRate);
    setChecked(ui->bitrateSwitchCheckBox, mSettingKeyCtx.bitrateSwitch);

    onFrameTypeChanged();
}

void xToolsCanBusStudioUi::initSettingSendCanFrame() {}

void xToolsCanBusStudioUi::onPluginChanged()
{
    int index = ui->pluginComboBox->currentIndex();
    mSettings->setValue(mSettingKeyCtx.pluginIndex, index);
}

void xToolsCanBusStudioUi::onDisconnectClicked()
{
    if (mDevice) {
        mDevice->disconnectDevice();
        mDevice->deleteLater();
        mDevice = Q_NULLPTR;
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
    mDevice = QCanBus::instance()->createDevice(pluginName, interfaceName, &errorString);
    if (!mDevice) {
        qCWarning(gLC) << errorString;
        return;
    }
#else
    mDevice = QCanBus::instance()->createDevice(pluginName.toLatin1(), interfaceName);
    if (!mDevice) {
        qCWarning(gLC) << tr("Create device failed!");
        return;
    }
#endif

    connect(mDevice, &QCanBusDevice::errorOccurred, this, &xToolsCanBusStudioUi::onErrorOccure);
    connect(mDevice, &QCanBusDevice::framesReceived, this, &xToolsCanBusStudioUi::onFrameReceived);
    connect(mDevice, &QCanBusDevice::framesWritten, this, &xToolsCanBusStudioUi::onFrameWritten);

    auto items = configurationItems();
    for (auto& item : items) {
        mDevice->setConfigurationParameter(item.first, item.second);
    }

    if (!mDevice->connectDevice()) {
        qCWarning(gLC) << tr("Connection error: %1").arg(mDevice->errorString());
        QMessageBox::warning(this,
                             tr("Connection Error"),
                             tr("Connection error: %1").arg(mDevice->errorString()));
        mDevice->deleteLater();
        mDevice = Q_NULLPTR;
        return;
    }

    mSettings->setValue(mSettingKeyCtx.interfaceName, interfaceName);
    updateUiState(true);
}

void xToolsCanBusStudioUi::onLoopbackIndexChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.loopback, index);
}

void xToolsCanBusStudioUi::onCustomConfigurationChanged()
{
    bool checked = ui->customConfigurationCheckBox->isChecked();
    setCustomConfigurationEnable(checked);
    mSettings->setValue(mSettingKeyCtx.customConfiguration, checked);
}

void xToolsCanBusStudioUi::onReceiveOwnIndexChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.receiveOwn, index);
}

void xToolsCanBusStudioUi::onCanFdIndexChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.canFd, index);
}

void xToolsCanBusStudioUi::onBitrateChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.bitrate, index);
}

void xToolsCanBusStudioUi::onDataBitrateChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.dataBitRate, index);
}

void xToolsCanBusStudioUi::onCustomBitrateChanged()
{
    bool checked = ui->customBitrateCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.customBitRate, checked);

    ui->bitrateComboBox->setEditable(checked);
}

void xToolsCanBusStudioUi::onCustomDataBitrateChanged()
{
    bool checked = ui->customDataBitrateCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.customDataBitRate, checked);

    ui->dataBitrateComboBox->setEditable(true);
}

void xToolsCanBusStudioUi::onFrameTypeChanged()
{
    int index = ui->frameTypeComboBox->currentIndex();
    mSettings->setValue(mSettingKeyCtx.frameTypeIndex, index);

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
    mSettings->setValue(mSettingKeyCtx.extendedFormat, checked);
}

void xToolsCanBusStudioUi::onFlexibleDataRateChanged()
{
    bool checked = ui->flexibleDataRateCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.flexibleDataRate, checked);

    ui->bitrateSwitchCheckBox->setEnabled(checked);
}

void xToolsCanBusStudioUi::onBitrateSwitchChanged()
{
    bool checked = ui->bitrateSwitchCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.bitrateSwitch, checked);
}

void xToolsCanBusStudioUi::onSendButtonClicked()
{
    if (!mDevice) {
        QString title = tr("Device is Not Ready");
        QString msg = tr("Device is not ready,"
                         " please connect the device then try angin!");
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

    if (mDevice->writeFrame(frame)) {
        QString view;
        if (frame.frameType() == QCanBusFrame::ErrorFrame) {
            view = mDevice->interpretErrorFrame(frame);
        } else {
            view = frame.toString();
        }

        QString flag = QString("<font color=green>[Tx] </font>");
        outputMessage(flag + view);
    } else {
        qCWarning(gLC) << mDevice->errorString();
    }
}

void xToolsCanBusStudioUi::onErrorOccure(QCanBusDevice::CanBusError error)
{
    if (mDevice) {
        Q_UNUSED(error);
        QMessageBox::warning(this, tr("Error Occure"), mDevice->errorString());
        onDisconnectClicked();
    }
}

void xToolsCanBusStudioUi::onFrameReceived()
{
    if (!mDevice) {
        return;
    }

    while (mDevice->framesAvailable()) {
        const QCanBusFrame frame = mDevice->readFrame();

        QString view;
        if (frame.frameType() == QCanBusFrame::ErrorFrame) {
            view = mDevice->interpretErrorFrame(frame);
        } else {
            view = frame.toString();
        }

        QString flag = QString("<font color=blue>[Rx] </font>");
        outputMessage(flag + view);
    }
}

void xToolsCanBusStudioUi::onFrameWritten(qint64 framesCount)
{
    qCInfo(gLC) << framesCount;
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
    int index = mSettings->value(key).toInt();
    if (index >= 0 && index <= cb->count() - 1) {
        cb->setCurrentIndex(index);
    }
}

void xToolsCanBusStudioUi::setChecked(QCheckBox* cb, const QString& key)
{
    if (cb) {
        bool checked = mSettings->value(key).toBool();
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
    ui->customBitrateCheckBox->setEnabled(enable);
    ui->customDataBitrateCheckBox->setEnabled(enable);
}

void xToolsCanBusStudioUi::outputMessage(const QString& msg)
{
    QString datetimeString =
#if 0
            QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
#else
        QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
#endif
    QString cookedMsg;
    cookedMsg = QString("<font color=silver>%1 </font>").arg(datetimeString);
    cookedMsg += msg;

    ui->textBrowser->append(cookedMsg);
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
