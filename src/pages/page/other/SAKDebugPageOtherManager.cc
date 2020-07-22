/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>

#include "SAKDebugPage.hh"
#include "SAKDebugPageOtherManager.hh"
#include "SAKTransmissionSettings.hh"
#include "SAKHighlightSettingsWidget.hh"
#include "SAKMoreSettingsWidget.hh"
#include "SAKReadWriteSettingsWidget.hh"
#include "SAKTimingSendingSettingsWidget.hh"
#include "SAKAutoResponseSettingsWidget.hh"

SAKDebugPageOtherManager::SAKDebugPageOtherManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
{
    transmissionSettings        = new SAKTransmissionSettings(_debugPage);
    autoResponseSettingWidget   = new SAKAutoResponseSettingsWidget(_debugPage);
    highlighterSettingPanel     = new SAKHighlightSettingsWidget(_debugPage->mOutputTextBroswer->document());
    mMoreSettingsWidget          = new SAKMoreSettingsWidget(_debugPage);
    readWriteSettingsWidget     = new SAKReadWriteSettingsWidget(_debugPage);
    timingSendingManager        = new SAKTimingSendingSettingsWidget(_debugPage);

    autoResponseSettingPushButton   = _debugPage->mAutoResponseSettingPushButton;
    highlightSettingPushButton      = _debugPage->mHighlightSettingPushButton;
    readWriteSettingPushButton      = _debugPage->mReadWriteSettingPushButton;
    transmissionSettingPushButton   = _debugPage->mTransmissionSettingPushButton;
    moreSettingsPushButton          = _debugPage->mMoreSettingsPushButton;
    timingSendingPushButton         = _debugPage->mTimingSendingPushButton;

    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherManager::onAutoresponseSettingPushbuttonClicked);
    connect(highlightSettingPushButton,    &QPushButton::clicked, this, &SAKDebugPageOtherManager::onHighlightSettingPushButtonClicked);
    connect(readWriteSettingPushButton,    &QPushButton::clicked, this, &SAKDebugPageOtherManager::onReadWriteSettingPushButtonClicked);
    connect(transmissionSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherManager::onTransmissionSettingPushButtonClicked);
    connect(moreSettingsPushButton,        &QPushButton::clicked, this, &SAKDebugPageOtherManager::onMoreSettingsPushButtonClicked);
    connect(timingSendingPushButton,       &QPushButton::clicked, this, &SAKDebugPageOtherManager::onTimingSendingPushButtonClicked);
}

SAKDebugPageOtherManager::~SAKDebugPageOtherManager()
{
    delete transmissionSettings;
    delete autoResponseSettingWidget;
    delete highlighterSettingPanel;
    delete mMoreSettingsWidget;
    delete readWriteSettingsWidget;
    delete timingSendingManager;

    transmissionSettings = Q_NULLPTR;
    autoResponseSettingWidget = Q_NULLPTR;
    highlighterSettingPanel = Q_NULLPTR;
    mMoreSettingsWidget = Q_NULLPTR;
    readWriteSettingsWidget = Q_NULLPTR;
    timingSendingManager = Q_NULLPTR;
}

SAKMoreSettingsWidget *SAKDebugPageOtherManager::moreSettingsWidget()
{
    return mMoreSettingsWidget;
}

void SAKDebugPageOtherManager::onAutoresponseSettingPushbuttonClicked()
{
    if (autoResponseSettingWidget->isHidden()){
        autoResponseSettingWidget->show();
    }else {
        autoResponseSettingWidget->activateWindow();
    }
}

void SAKDebugPageOtherManager::onHighlightSettingPushButtonClicked()
{

    if (highlighterSettingPanel->isHidden()){
        highlighterSettingPanel->show();
    }else {
        highlighterSettingPanel->activateWindow();
    }
}

void SAKDebugPageOtherManager::onReadWriteSettingPushButtonClicked()
{
    if (readWriteSettingsWidget->isHidden()){
        readWriteSettingsWidget->show();
    }else {
        readWriteSettingsWidget->activateWindow();
    }
}

void SAKDebugPageOtherManager::onTransmissionSettingPushButtonClicked()
{
    if (transmissionSettings->isHidden()){
        transmissionSettings->show();
    }else {
        transmissionSettings->activateWindow();
    }
}

void SAKDebugPageOtherManager::onMoreSettingsPushButtonClicked()
{
    if (mMoreSettingsWidget->isHidden()){
        mMoreSettingsWidget->show();
    }else {
        mMoreSettingsWidget->activateWindow();
    }
}

void SAKDebugPageOtherManager::onTimingSendingPushButtonClicked()
{
    if (timingSendingManager->isHidden()){
        timingSendingManager->show();
    }else{
        timingSendingManager->activateWindow();
    }
}
