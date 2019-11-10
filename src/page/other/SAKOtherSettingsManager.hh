/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKOTHERSETTINGS_HH
#define SAKOTHERSETTINGS_HH

#include <QObject>
#include <QPushButton>

class TransmissionSettings;
class AutoResponseSettingWidget;

class SAKDebugPage;
class FormatSettingsWidget;
class HighlightSettingsWidget;
class MoreOtherSettingsWidget;
class ReadWriteSettingsWidget;
class SAKTimingSendingManager;

class SAKOtherSettings:public QObject
{
    Q_OBJECT
public:
    SAKOtherSettings(SAKDebugPage *debugPage, QObject *parent = nullptr);
    ~SAKOtherSettings();

private:    
    QPushButton                 *autoResponseSettingPushButton  = nullptr;  // 自动回复设置面板调出按钮
    QPushButton                 *highlightSettingPushButton     = nullptr;  // 高亮设置面板调出按钮
    QPushButton                 *readWriteSettingPushButton     = nullptr;  // 读写设置面板调出按钮
    QPushButton                 *transmissionSettingPushButton  = nullptr;  // 数据转发设置面板调出按钮
    QPushButton                 *moreOtherSettingsPushButton    = nullptr;  // 更多其他设置按钮
    QPushButton                 *timingSendPushButton           = nullptr;  // 自动会回复

    TransmissionSettings        *transmissionSettings;                      // 数据转发设置面板
    AutoResponseSettingWidget   *autoResponseSettingWidget;                 // 自动回复设置面板
    HighlightSettingsWidget     *highlighterSettingPanel;                   // 高亮设置面板
    MoreOtherSettingsWidget     *moreOtherSettingsWidget;                   // 更多设置面板
    FormatSettingsWidget        *formatSettingsWidget;                      // 格式分析面板
    ReadWriteSettingsWidget     *readWriteSettingsWidget;                   // 读写参数设置面板
    SAKTimingSendingManager     *timingSendingManager;                      // 定时发送设置面板

    SAKDebugPage                *_debugPage                     = nullptr;  // 调试页面
private slots:
    void onAutoresponseSettingPushbuttonClicked();
    void onHighlightSettingPushButtonClicked();
    void onReadWriteSettingPushButtonClicked();
    void onTransmissionSettingPushButtonClicked();
    void onMoreOtherSettingsPushButtonClicked();
    void onFormatSettingsPushButtonClicked();
    void onTimingSendPushButtonClicked();
};

#endif
