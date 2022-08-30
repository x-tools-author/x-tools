/******************************************************************************
 * Copyright 2022 Qter(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QCanBus>
#include <QVector>
#include <QCheckBox>
#include <QLineEdit>
#include <QDateTime>
#include <QMessageBox>
#include <QLoggingCategory>

#include "SAKCanBusDebugger.hh"
#include "ui_SAKCanBusDebugger.h"

const QLoggingCategory gLC("SAK.CANStudio");

SAKCanBusDebugger::SAKCanBusDebugger(QSettings *settings,
                           const QString settingsGroup,
                           QSqlDatabase *sqlDatabase,
                           QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKCanBusDebugger)
    , mSettings(settings)
    , mDevice(Q_NULLPTR)
{
    Q_UNUSED(settingsGroup);
    Q_UNUSED(sqlDatabase);
    if (!mSettings) {
        mSettings = new QSettings("SAKCanBusStudio.ini",
                                  QSettings::IniFormat, this);
    }

    ui->setupUi(this);
    initUi();
    initSetting();

    // Device is not connected.
    updateUiState(false);
}

SAKCanBusDebugger::~SAKCanBusDebugger()
{
    delete ui;
}

void SAKCanBusDebugger::initUi()
{
    initUiSelectPlugin();
    initUiSpecifyConfiguration();
    initUiCanFrame();
    initUiSendCanFrame();
}

void SAKCanBusDebugger::initUiSelectPlugin()
{
    ui->pluginComboBox->clear();
    ui->pluginComboBox->addItems(QCanBus::instance()->plugins());
    ui->disconnectPushButton->setEnabled(false);
    ui->connectPushButton->setEnabled(true);

    connect(ui->pluginComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onPluginChanged);
    connect(ui->disconnectPushButton, &QPushButton::clicked,
            this, &SAKCanBusDebugger::onDisconnectClicked);
    connect(ui->connectPushButton, &QPushButton::clicked,
            this, &SAKCanBusDebugger::onConnectClicked);
}

void SAKCanBusDebugger::initUiSpecifyConfiguration()
{
    setOptions(ui->loopbackComboBox, true);
    setOptions(ui->receivOwnComboBox, true);
    setOptions(ui->canFdComboBox, false);
    setBitRates(ui->bitrateComboBox, false);
    setBitRates(ui->dataBitrateComboBox, true);

    ui->interfaceNameComboBox->lineEdit()->setPlaceholderText(tr("can0"));

    connect(ui->customConfigurationCheckBox, &QCheckBox::clicked,
            this, &SAKCanBusDebugger::onCustomConfigurationChanged);
    connect(ui->loopbackComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onLoopbackIndexChanged);
    connect(ui->receivOwnComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onReceiveOwnIndexChanged);
    connect(ui->canFdComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onCanFdIndexChanged);
    connect(ui->bitrateComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onBitrateChanged);
    connect(ui->dataBitrateComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onDataBitrateChanged);
    connect(ui->customBitrateCheckBox, &QCheckBox::clicked,
            this, &SAKCanBusDebugger::onCustomBitrateChanged);
    connect(ui->customDataBitrateCheckBox, &QCheckBox::clicked,
            this, &SAKCanBusDebugger::onCustomDataBitrateChanged);
}

void SAKCanBusDebugger::initUiCanFrame()
{
    ui->frameTypeComboBox->clear();
    ui->frameTypeComboBox->addItem(tr("DataFrame"),
                                   QCanBusFrame::DataFrame);
    ui->frameTypeComboBox->addItem(tr("ErrorFrame"),
                                   QCanBusFrame::ErrorFrame);
    ui->frameTypeComboBox->addItem(tr("RemoteRequestFrame"),
                                   QCanBusFrame::RemoteRequestFrame);

    connect(ui->frameTypeComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKCanBusDebugger::onFrameTypeChanged);
    connect(ui->extendedFormatCheckBox, &QCheckBox::clicked,
            this, &SAKCanBusDebugger::onExtendedFormatChanged);
    connect(ui->flexibleDataRateCheckBox, &QCheckBox::clicked,
            this, &SAKCanBusDebugger::onFlexibleDataRateChanged);
    connect(ui->bitrateSwitchCheckBox, &QCheckBox::clicked,
            this, &SAKCanBusDebugger::onBitrateSwitchChanged);
}

void SAKCanBusDebugger::initUiSendCanFrame()
{
    const QString inputTips = tr("Hex");
    ui->frameIdComboBox->lineEdit()->setPlaceholderText(inputTips);
    ui->payloadComboBox->lineEdit()->setPlaceholderText(inputTips);

    connect(ui->sendPushButton, &QPushButton::clicked,
            this, &SAKCanBusDebugger::onSendButtonClicked);
}

void SAKCanBusDebugger::initSetting()
{
    initSettingSelectPlugin();
    initSettingSpecifyConfiguration();
    initSettingCanFrame();
    initSettingSendCanFrame();
}

void SAKCanBusDebugger::initSettingSelectPlugin()
{
    setCurrentIndex(ui->pluginComboBox, mSettingKeyCtx.pluginIndex);
}

void SAKCanBusDebugger::initSettingSpecifyConfiguration()
{
    QString name = mSettings->value(mSettingKeyCtx.interfaceName).toString();
    ui->interfaceNameComboBox->lineEdit()->setText(name);

    setChecked(ui->customConfigurationCheckBox,
               mSettingKeyCtx.customConfiguration);
    setCurrentIndex(ui->loopbackComboBox, mSettingKeyCtx.loopback);
    setCurrentIndex(ui->receivOwnComboBox, mSettingKeyCtx.receiveOwn);
    setCurrentIndex(ui->bitrateComboBox, mSettingKeyCtx.bitrate);
    setCurrentIndex(ui->canFdComboBox, mSettingKeyCtx.canFd);
    setCurrentIndex(ui->dataBitrateComboBox, mSettingKeyCtx.dataBitRate);
    setChecked(ui->customBitrateCheckBox, mSettingKeyCtx.customBitRate);
    setChecked(ui->customDataBitrateCheckBox,
               mSettingKeyCtx.customDataBitRate);

    bool enable =
            mSettings->value(mSettingKeyCtx.customConfiguration).toBool();
    setCustomConfigurationEnable(enable);
}

void SAKCanBusDebugger::initSettingCanFrame()
{
    setCurrentIndex(ui->frameTypeComboBox, mSettingKeyCtx.frameTypeIndex);
    setChecked(ui->extendedFormatCheckBox, mSettingKeyCtx.extendedFormat);
    setChecked(ui->flexibleDataRateCheckBox, mSettingKeyCtx.flexibleDataRate);
    setChecked(ui->bitrateSwitchCheckBox, mSettingKeyCtx.bitrateSwitch);

    onFrameTypeChanged();
}

void SAKCanBusDebugger::initSettingSendCanFrame()
{

}

void SAKCanBusDebugger::onPluginChanged()
{
    int index = ui->pluginComboBox->currentIndex();
    mSettings->setValue(mSettingKeyCtx.pluginIndex, index);
}

void SAKCanBusDebugger::onDisconnectClicked()
{
    if (mDevice) {
        mDevice->disconnectDevice();
        mDevice->deleteLater();
        mDevice = Q_NULLPTR;
    }

    updateUiState(false);
}

void SAKCanBusDebugger::onConnectClicked()
{
    const QString pluginName = ui->pluginComboBox->currentText();
    const QString interfaceName = ui->interfaceNameComboBox->currentText();

    if (interfaceName.isEmpty()) {
        QMessageBox::warning(this, tr("Interface Name is Empty"),
                             tr("Interface name is empty, "
                                "please input the name then try again!"));
        return;
    }

    QString errorString;
    mDevice = QCanBus::instance()->createDevice(
                pluginName, interfaceName, &errorString);
    if (!mDevice) {
        qCWarning(gLC) << errorString;
        return;
    }

    connect(mDevice, &QCanBusDevice::errorOccurred,
            this, &SAKCanBusDebugger::onErrorOccure);
    connect(mDevice, &QCanBusDevice::framesReceived,
            this, &SAKCanBusDebugger::onFrameReceived);
    connect(mDevice, &QCanBusDevice::framesWritten,
            this, &SAKCanBusDebugger::onFrameWritten);

    auto items = configurationItems();
    for (const ConfigurationItem &item : items) {
        mDevice->setConfigurationParameter(item.first, item.second);
    }

    if (!mDevice->connectDevice()) {
        qCWarning(gLC)
                << tr("Connection error: %1").arg(mDevice->errorString());
        QMessageBox::warning(
                    this, tr("Connection Error"),
                    tr("Connection error: %1").arg(mDevice->errorString()));
        mDevice->deleteLater();
        mDevice = Q_NULLPTR;
        return;
    }

    mSettings->setValue(mSettingKeyCtx.interfaceName, interfaceName);
    updateUiState(true);
}

void SAKCanBusDebugger::onLoopbackIndexChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.loopback, index);
}

void SAKCanBusDebugger::onCustomConfigurationChanged()
{
    bool checked = ui->customConfigurationCheckBox->isChecked();
    setCustomConfigurationEnable(checked);
    mSettings->setValue(mSettingKeyCtx.customConfiguration, checked);
}

void SAKCanBusDebugger::onReceiveOwnIndexChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.receiveOwn, index);
}

void SAKCanBusDebugger::onCanFdIndexChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.canFd, index);
}

void SAKCanBusDebugger::onBitrateChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.bitrate, index);
}

void SAKCanBusDebugger::onDataBitrateChanged(int index)
{
    mSettings->setValue(mSettingKeyCtx.dataBitRate, index);
}

void SAKCanBusDebugger::onCustomBitrateChanged()
{
    bool checked = ui->customBitrateCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.customBitRate, checked);

    ui->bitrateComboBox->setEditable(checked);
}

void SAKCanBusDebugger::onCustomDataBitrateChanged()
{
    bool checked = ui->customDataBitrateCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.customDataBitRate, checked);

    ui->dataBitrateComboBox->setEditable(true);
}

void SAKCanBusDebugger::onFrameTypeChanged()
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

void SAKCanBusDebugger::onExtendedFormatChanged()
{
    bool checked = ui->extendedFormatCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.extendedFormat, checked);
}

void SAKCanBusDebugger::onFlexibleDataRateChanged()
{
    bool checked = ui->flexibleDataRateCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.flexibleDataRate, checked);

    ui->bitrateSwitchCheckBox->setEnabled(checked);
}

void SAKCanBusDebugger::onBitrateSwitchChanged()
{
    bool checked = ui->bitrateSwitchCheckBox->isChecked();
    mSettings->setValue(mSettingKeyCtx.bitrateSwitch, checked);
}

void SAKCanBusDebugger::onSendButtonClicked()
{
    if (!mDevice) {
        QString title = tr("Device is Not Ready");
        QString msg = tr("Device is not ready,"
                         " please connect the device then try angin!");
        QMessageBox::warning(this, title, msg);
        return;
    }

    const uint frameId =
            ui->frameIdComboBox->currentText().toUInt(Q_NULLPTR, 16);
    QString data = ui->payloadComboBox->currentText().trimmed();
    const QByteArray payload =
            QByteArray::fromHex(data.remove(QLatin1Char(' ')).toLatin1());

    QCanBusFrame frame = QCanBusFrame(frameId, payload);
    frame.setExtendedFrameFormat(ui->extendedFormatCheckBox->isChecked());

    if (ui->flexibleDataRateCheckBox->isEnabled()) {
        frame.setFlexibleDataRateFormat(
                    ui->flexibleDataRateCheckBox->isChecked());
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

void SAKCanBusDebugger::onErrorOccure(QCanBusDevice::CanBusError error)
{
    if (mDevice) {
        Q_UNUSED(error);
        QMessageBox::warning(this, tr("Error Occure"), mDevice->errorString());
        onDisconnectClicked();
    }
}

void SAKCanBusDebugger::onFrameReceived()
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

void SAKCanBusDebugger::onFrameWritten(qint64 framesCount)
{
    qCInfo(gLC) << framesCount;
}

void SAKCanBusDebugger::setOptions(QComboBox *cb, bool usingUnspecified)
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

void SAKCanBusDebugger::setCurrentIndex(QComboBox *cb, const QString &key)
{
    int index = mSettings->value(key).toInt();
    if (index >= 0 && index <= cb->count() - 1) {
        cb->setCurrentIndex(index);
    }
}

void SAKCanBusDebugger::setChecked(QCheckBox *cb, const QString &key)
{
    if (cb) {
        bool checked = mSettings->value(key).toBool();
        cb->setChecked(checked);
    }
}

void SAKCanBusDebugger::setCustomConfigurationEnable(bool enable)
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

void SAKCanBusDebugger::outputMessage(const QString &msg)
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

void SAKCanBusDebugger::updateUiState(bool connected)
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

QVector<SAKCanBusDebugger::ConfigurationItem> SAKCanBusDebugger::configurationItems()
{
    QVector<SAKCanBusDebugger::ConfigurationItem> items;
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

void SAKCanBusDebugger::setBitRates(QComboBox *cb, bool isFlexibleDataRateEnable)
{
    if (!cb) {
        return;
    }

    const QVector<int> rates = {
        10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000
    };

    const QVector<int> dataRates = {
        2000000, 4000000, 8000000
    };

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
