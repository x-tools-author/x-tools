/******************************************************************************
 * Copyright 2022 Qter(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCANBUSSTUDIO_HH
#define SAKCANBUSSTUDIO_HH

#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QCanBusFrame>
#include <QCanBusDevice>

QT_BEGIN_NAMESPACE
namespace Ui { class SAKCanBusStudio; }
QT_END_NAMESPACE

class QSqlDatabase;
class SAKCanBusStudio : public QWidget
{
    Q_OBJECT
    typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;
public:
    Q_INVOKABLE SAKCanBusStudio(QSettings *settings,
                             const QString settingsGroup = QString(),
                             QSqlDatabase *sqlDatabase = Q_NULLPTR,
                             QWidget *parent = Q_NULLPTR);
    ~SAKCanBusStudio();
private:
    struct {
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
    } mSettingKeyCtx;
private:
    Ui::SAKCanBusStudio *ui;
    QSettings *mSettings;
    QCanBusDevice *mDevice;
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


    // These are slots.
    void onPluginChanged();
    void onDisconnectClicked();
    void onConnectClicked();

    void onCustomConfigurationChanged();
    void onLoopbackIndexChanged(int index);
    void onReceiveOwnIndexChanged(int index);
    void onCanFdIndexChanged(int index);
    void onBitrateChanged(int index);
    void onDataBitrateChanged(int index);
    void onCustomBitrateChanged();
    void onCustomDataBitrateChanged();

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

#endif // SAKCANBUSSTUDIO_H
