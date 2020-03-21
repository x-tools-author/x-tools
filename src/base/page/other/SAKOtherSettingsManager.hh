/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKOTHERSETTINGSMANAGER_HH
#define SAKOTHERSETTINGSMANAGER_HH

#include <QObject>
#include <QPushButton>

class SAKTransmissionSettings;
class SAKAutoResponseSettingsWidget;

class SAKDebugPage;
class SAKHighlightSettingsWidget;
class SAKMoreSettingsWidget;
class SAKReadWriteSettingsWidget;
class SAKTimingSendingSettingsWidget;

class SAKOtherSettingsManager:public QObject
{
    Q_OBJECT
public:
    SAKOtherSettingsManager(SAKDebugPage *debugPage, QObject *parent = nullptr);
    ~SAKOtherSettingsManager();

private:
    QPushButton *transmissionSettingPushButton;
    QPushButton *readWriteSettingPushButton;
    QPushButton *autoResponseSettingPushButton;
    QPushButton *chartSettingPushButton;
    QPushButton *timingSendingPushButton;
    QPushButton *highlightSettingPushButton;
    QPushButton *moreSettingsPushButton;

    SAKTransmissionSettings        *transmissionSettings;
    SAKReadWriteSettingsWidget     *readWriteSettingsWidget;
    SAKAutoResponseSettingsWidget   *autoResponseSettingWidget;
    SAKTimingSendingSettingsWidget     *timingSendingManager;
    SAKHighlightSettingsWidget     *highlighterSettingPanel;
    SAKMoreSettingsWidget       *moreSettingsWidget;

    SAKDebugPage                *_debugPage;
private slots:
    void onTransmissionSettingPushButtonClicked();
    void onReadWriteSettingPushButtonClicked();
    void onAutoresponseSettingPushbuttonClicked();
    void onTimingSendingPushButtonClicked();
    void onHighlightSettingPushButtonClicked();    
    void onMoreSettingsPushButtonClicked();
};

#endif
