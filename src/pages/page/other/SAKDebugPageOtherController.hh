/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEOTHERSCONTROLLER_HH
#define SAKDEBUGPAGEOTHERSCONTROLLER_HH

#include <QObject>
#include <QPushButton>

class SAKDebugPage;
class SAKTransmissionSettings;
class SAKHighlightSettingsWidget;
class SAKOtherTimingSentItemManager;
class SAKOtherAutoResponseItemManager;

/// @brief other setting module
class SAKDebugPageOtherController:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageOtherController(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageOtherController();
private:
    SAKDebugPage *mDebugPage;

    QPushButton *moreSettingsPushButton;
    QPushButton *timingSendingPushButton;
    QPushButton *highlightSettingPushButton;
    QPushButton *readWriteSettingPushButton;
    QPushButton *autoResponseSettingPushButton;
    QPushButton *transmissionSettingPushButton;

    SAKTransmissionSettings *mTransmissionSettings;
    SAKHighlightSettingsWidget *mHighlightSettingsWidget;
    SAKOtherAutoResponseItemManager *mAutoResponseSettingWidget;
    SAKOtherTimingSentItemManager *mTimingSendingSettingsWidget;
private slots:
    void onTimingSendingPushButtonClicked();
    void onHighlightSettingPushButtonClicked();
    void onAutoresponseSettingPushbuttonClicked();
    void onTransmissionSettingPushButtonClicked();
};

#endif
