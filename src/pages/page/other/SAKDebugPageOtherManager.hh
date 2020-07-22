/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEOTHERSMANAGER_HH
#define SAKDEBUGPAGEOTHERSMANAGER_HH

#include <QObject>
#include <QPushButton>

class SAKTransmissionSettings;
class SAKAutoResponseSettingsWidget;

class SAKDebugPage;
class SAKHighlightSettingsWidget;
class SAKMoreSettingsWidget;
class SAKReadWriteSettingsWidget;
class SAKTimingSendingSettingsWidget;

class SAKDebugPageOtherManager:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageOtherManager(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageOtherManager();

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
