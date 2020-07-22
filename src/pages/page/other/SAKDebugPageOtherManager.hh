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

class SAKDebugPage;
class SAKMoreSettingsWidget;
class SAKTransmissionSettings;
class SAKHighlightSettingsWidget;
class SAKReadWriteSettingsWidget;
class SAKAutoResponseSettingsWidget;
class SAKTimingSendingSettingsWidget;

/// @brief other setting module
class SAKDebugPageOtherManager:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageOtherManager(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageOtherManager();

    /**
     * @brief moreSettingsWidget: get the more settings widget
     * @return widget: setting widget
     */
    SAKMoreSettingsWidget *moreSettingsWidget();
private:
    SAKDebugPage *mDebugPage;

    QPushButton *moreSettingsPushButton;
    QPushButton *timingSendingPushButton;
    QPushButton *highlightSettingPushButton;
    QPushButton *readWriteSettingPushButton;
    QPushButton *autoResponseSettingPushButton;
    QPushButton *transmissionSettingPushButton;

    SAKMoreSettingsWidget *mMoreSettingsWidget;
    SAKTransmissionSettings *mTransmissionSettings;
    SAKHighlightSettingsWidget *mHighlightSettingsWidget;
    SAKReadWriteSettingsWidget *mReadWriteSettingsWidget;
    SAKAutoResponseSettingsWidget *mAutoResponseSettingWidget;
    SAKTimingSendingSettingsWidget *mTimingSendingSettingsWidget;
private slots:
    void onMoreSettingsPushButtonClicked();
    void onTimingSendingPushButtonClicked();
    void onReadWriteSettingPushButtonClicked();
    void onHighlightSettingPushButtonClicked();
    void onAutoresponseSettingPushbuttonClicked();
    void onTransmissionSettingPushButtonClicked();
};

#endif
