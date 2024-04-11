/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCanBusDevice>
#include <QCanBusDeviceInfo>
#include <QCanBusFrame>
#include <QCheckBox>
#include <QComboBox>
#include <QSettings>
#include <QVector>
#include <QWidget>

namespace Ui {
class xToolsCanBusStudioUi;
}
class xToolsCanBusStudioUi : public QWidget
{
    Q_OBJECT
    typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;

public:
    Q_INVOKABLE xToolsCanBusStudioUi(QWidget *parent = Q_NULLPTR);
    ~xToolsCanBusStudioUi();

private:
    struct
    {
        const QString pluginIndex = "CANStudio/pluginIndex";

        const QString interfaceName = "CANStudio/interfaceName";
        const QString customConfiguration = "CANStudio/customConfiguration";
        const QString loopback = "CANStudio/loopback";
        const QString receiveOwn = "CANStudio/receiveOwn";
        const QString canFd = "CANStudio/canFd";
        const QString bitrate = "CANStudio/bitrate";
        const QString dataBitRate = "CANStudio/dataBitRate";
        const QString customBitRate = "CANStudio/customBitRate";
        const QString customDataBitRate = "CANStudio/customDataBitRate";

        const QString frameTypeIndex = "CANStudio/frameTypeIndex";
        const QString extendedFormat = "CANStudio/extendedFormat";
        const QString flexibleDataRate = "CANStudio/fleibleDataRate";
        const QString bitrateSwitch = "CANStudio/bitrateSwitch";
    } m_settingKeyCtx;

private:
    Ui::xToolsCanBusStudioUi *ui;
    QSettings *m_settings{nullptr};
    QCanBusDevice *m_device{nullptr};
    QList<QCanBusDeviceInfo> m_interfaces;

private:
    void initUi();
    void initUiSelectPlugin();
    void initUiSpecifyConfiguration();
    void initUiCanFrame();
    void initUiSendCanFrame();

    void initSetting();
    void initSettingSelectPlugin();
    void initSettingSpecifyConfiguration();
    void initSettingCanFrame();
    void initSettingSendCanFrame();

private slots:
    // These are slots.
    void onPluginChanged(QString plugin);
    void onDisconnectClicked();
    void onConnectClicked();

    void onCustomConfigurationChanged();
    void onLoopbackIndexChanged(int index);
    void onReceiveOwnIndexChanged(int index);
    void onCanFdIndexChanged(int index);
    void onBitrateChanged(int index);
    void onDataBitrateChanged(int index);

    void onFrameTypeChanged();
    void onExtendedFormatChanged();
    void onFlexibleDataRateChanged();
    void onBitrateSwitchChanged();

    void onSendButtonClicked();

    // Slots about CAN bus device
    void onErrorOccure(QCanBusDevice::CanBusError error);
    void onFrameReceived();
    void onFrameWritten(qint64 framesCount);

private:
    void setOptions(QComboBox *cb, bool usingUnspecified);
    void setCurrentIndex(QComboBox *cb, const QString &key);
    void setBitRates(QComboBox *cb, bool isFlexibleDataRateEnable);
    void setChecked(QCheckBox *cb, const QString &key);
    void setCustomConfigurationEnable(bool enable);
    void outputMessage(const QString &msg);
    void updateUiState(bool connected);
    QVector<ConfigurationItem> configurationItems();
};
