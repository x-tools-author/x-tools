/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    SAKOtherSettingsManager(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKOtherSettingsManager();

    /**
     * @brief moreSettingsWidget 获取“更多设置”控制面板
     * @return “更多设置控制面板”
     */
    SAKMoreSettingsWidget *moreSettingsWidget();
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
    SAKMoreSettingsWidget *mMoreSettingsWidget;

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
