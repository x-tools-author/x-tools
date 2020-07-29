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

#include "SAKSettings.hh"
#include "SAKDebugPage.hh"
#include "SAKOtherTransmissionPageViewer.hh"
#include "SAKOtherHighlighterManager.hh"
#include "SAKDebugPageOtherController.hh"
#include "SAKOtherAutoResponseItemManager.hh"
#include "SAKOtherTimingSentItemManager.hh"
#include "SAKOtherAnalyzerThreadManager.hh"

SAKDebugPageOtherController::SAKDebugPageOtherController(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,mDebugPage (debugPage)
{
    mTransmissionSettings = new SAKOtherTransmissionPageViewer(mDebugPage);
    mHighlightSettingsWidget = new SAKOtherHighlighterManager(mDebugPage->mOutputTextBroswer->document());
    mAutoResponseSettingWidget = new SAKOtherAutoResponseItemManager(mDebugPage);
    mTimingSendingSettingsWidget = new SAKOtherTimingSentItemManager(mDebugPage);
    mAnalyzerThreadManager = new SAKOtherAnalyzerThreadManager(SAKSettings::instance());

    moreSettingsPushButton = mDebugPage->mMoreSettingsPushButton;
    timingSendingPushButton = mDebugPage->mTimingSendingPushButton;
    highlightSettingPushButton = mDebugPage->mHighlightSettingPushButton;
    mAnalyzerPushButton = mDebugPage->mAnalyzerPushButton;
    autoResponseSettingPushButton = mDebugPage->mAutoResponseSettingPushButton;
    transmissionSettingPushButton = mDebugPage->mTransmissionSettingPushButton;

    connect(timingSendingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onTimingSendingPushButtonClicked);
    connect(highlightSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onHighlightSettingPushButtonClicked);
    connect(mAnalyzerPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onAnalyzerPushButtonClicked);
    connect(transmissionSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onTransmissionSettingPushButtonClicked);
    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onAutoresponseSettingPushbuttonClicked);
}

SAKDebugPageOtherController::~SAKDebugPageOtherController()
{
    delete mTransmissionSettings;
    delete mHighlightSettingsWidget;
    delete mAutoResponseSettingWidget;
    delete mTimingSendingSettingsWidget;
    delete mAnalyzerThreadManager;

    mTransmissionSettings = Q_NULLPTR;
    mAnalyzerThreadManager = Q_NULLPTR;
    mHighlightSettingsWidget = Q_NULLPTR;
    mAutoResponseSettingWidget = Q_NULLPTR;
    mTimingSendingSettingsWidget = Q_NULLPTR;
}

SAKOtherAnalyzerThreadManager *SAKDebugPageOtherController::analyzerThreadManager()
{
    return mAnalyzerThreadManager;
}

void SAKDebugPageOtherController::onTimingSendingPushButtonClicked()
{
    if (mTimingSendingSettingsWidget->isHidden()){
        mTimingSendingSettingsWidget->show();
    }else{
        mTimingSendingSettingsWidget->activateWindow();
    }
}

void SAKDebugPageOtherController::onHighlightSettingPushButtonClicked()
{

    if (mHighlightSettingsWidget->isHidden()){
        mHighlightSettingsWidget->show();
    }else {
        mHighlightSettingsWidget->activateWindow();
    }
}

void SAKDebugPageOtherController::onAnalyzerPushButtonClicked()
{
    if (mAnalyzerThreadManager->isHidden()){
        mAnalyzerThreadManager->show();
    }else{
        mAnalyzerThreadManager->activateWindow();
    }
}

void SAKDebugPageOtherController::onTransmissionSettingPushButtonClicked()
{
    if (mTransmissionSettings->isHidden()){
        mTransmissionSettings->show();
    }else {
        mTransmissionSettings->activateWindow();
    }
}

void SAKDebugPageOtherController::onAutoresponseSettingPushbuttonClicked()
{
    if (mAutoResponseSettingWidget->isHidden()){
        mAutoResponseSettingWidget->show();
    }else {
        mAutoResponseSettingWidget->activateWindow();
    }
}
