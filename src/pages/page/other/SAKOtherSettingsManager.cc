/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.

 *


 */
#include <QDebug>

#include "SAKDebugPage.hh"
#include "SAKOtherSettingsManager.hh"
#include "SAKTransmissionSettings.hh"
#include "SAKHighlightSettingsWidget.hh"
#include "SAKMoreSettingsWidget.hh"
#include "SAKReadWriteSettingsWidget.hh"
#include "SAKTimingSendingSettingsWidget.hh"
#include "SAKAutoResponseSettingsWidget.hh"

SAKOtherSettingsManager::SAKOtherSettingsManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
{
    transmissionSettings        = new SAKTransmissionSettings(_debugPage);
    autoResponseSettingWidget   = new SAKAutoResponseSettingsWidget(_debugPage);
    highlighterSettingPanel     = new SAKHighlightSettingsWidget(_debugPage->outputTextBroswer->document());
    moreSettingsWidget          = new SAKMoreSettingsWidget(_debugPage);
    readWriteSettingsWidget     = new SAKReadWriteSettingsWidget(_debugPage);
    timingSendingManager        = new SAKTimingSendingSettingsWidget(_debugPage);

    autoResponseSettingPushButton   = _debugPage->autoResponseSettingPushButton;
    highlightSettingPushButton      = _debugPage->highlightSettingPushButton;
    readWriteSettingPushButton      = _debugPage->readWriteSettingPushButton;
    transmissionSettingPushButton   = _debugPage->transmissionSettingPushButton;
    moreSettingsPushButton          = _debugPage->moreSettingsPushButton;
    timingSendingPushButton         = _debugPage->timingSendingPushButton;

    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKOtherSettingsManager::onAutoresponseSettingPushbuttonClicked);
    connect(highlightSettingPushButton,    &QPushButton::clicked, this, &SAKOtherSettingsManager::onHighlightSettingPushButtonClicked);
    connect(readWriteSettingPushButton,    &QPushButton::clicked, this, &SAKOtherSettingsManager::onReadWriteSettingPushButtonClicked);
    connect(transmissionSettingPushButton, &QPushButton::clicked, this, &SAKOtherSettingsManager::onTransmissionSettingPushButtonClicked);
    connect(moreSettingsPushButton,        &QPushButton::clicked, this, &SAKOtherSettingsManager::onMoreSettingsPushButtonClicked);
    connect(timingSendingPushButton,       &QPushButton::clicked, this, &SAKOtherSettingsManager::onTimingSendingPushButtonClicked);
}

SAKOtherSettingsManager::~SAKOtherSettingsManager()
{
    delete transmissionSettings;
    delete autoResponseSettingWidget;
    delete highlighterSettingPanel;
    delete moreSettingsWidget;
    delete readWriteSettingsWidget;
    delete timingSendingManager;

    transmissionSettings = Q_NULLPTR;
    autoResponseSettingWidget = Q_NULLPTR;
    highlighterSettingPanel = Q_NULLPTR;
    moreSettingsWidget = Q_NULLPTR;
    readWriteSettingsWidget = Q_NULLPTR;
    timingSendingManager = Q_NULLPTR;
}

void SAKOtherSettingsManager::onAutoresponseSettingPushbuttonClicked()
{
    if (autoResponseSettingWidget->isHidden()){
        autoResponseSettingWidget->show();
    }else {
        autoResponseSettingWidget->activateWindow();
    }
}

void SAKOtherSettingsManager::onHighlightSettingPushButtonClicked()
{

    if (highlighterSettingPanel->isHidden()){
        highlighterSettingPanel->show();
    }else {
        highlighterSettingPanel->activateWindow();
    }
}

void SAKOtherSettingsManager::onReadWriteSettingPushButtonClicked()
{
    if (readWriteSettingsWidget->isHidden()){
        readWriteSettingsWidget->show();
    }else {
        readWriteSettingsWidget->activateWindow();
    }
}

void SAKOtherSettingsManager::onTransmissionSettingPushButtonClicked()
{
    if (transmissionSettings->isHidden()){
        transmissionSettings->show();
    }else {
        transmissionSettings->activateWindow();
    }
}

void SAKOtherSettingsManager::onMoreSettingsPushButtonClicked()
{
    if (moreSettingsWidget->isHidden()){
        moreSettingsWidget->show();
    }else {
        moreSettingsWidget->activateWindow();
    }
}

void SAKOtherSettingsManager::onTimingSendingPushButtonClicked()
{
    if (timingSendingManager->isHidden()){
        timingSendingManager->show();
    }else{
        timingSendingManager->activateWindow();
    }
}
