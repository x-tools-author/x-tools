/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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

SAKMoreSettingsWidget *SAKOtherSettingsManager::moreSettingsWidget()
{
    return mMoreSettingsWidget;
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
    if (mMoreSettingsWidget->isHidden()){
        mMoreSettingsWidget->show();
    }else {
        mMoreSettingsWidget->activateWindow();
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
