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
#include "SAKMoreSettingsWidget.hh"
#include "SAKTransmissionSettings.hh"
#include "SAKHighlightSettingsWidget.hh"
#include "SAKReadWriteSettingsWidget.hh"
#include "SAKDebugPageOtherController.hh"
#include "SAKAutoResponseSettingsWidget.hh"
#include "SAKTimingSendingSettingsWidget.hh"

SAKDebugPageOtherController::SAKDebugPageOtherController(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,mDebugPage (debugPage)
{
    mMoreSettingsWidget = new SAKMoreSettingsWidget(mDebugPage);
    mTransmissionSettings = new SAKTransmissionSettings(mDebugPage);
    mHighlightSettingsWidget = new SAKHighlightSettingsWidget(mDebugPage->mOutputTextBroswer->document());
    mReadWriteSettingsWidget = new SAKReadWriteSettingsWidget(mDebugPage);
    mAutoResponseSettingWidget = new SAKAutoResponseSettingsWidget(mDebugPage);
    mTimingSendingSettingsWidget = new SAKTimingSendingSettingsWidget(mDebugPage);

    moreSettingsPushButton = mDebugPage->mMoreSettingsPushButton;
    timingSendingPushButton = mDebugPage->mTimingSendingPushButton;
    highlightSettingPushButton = mDebugPage->mHighlightSettingPushButton;
    readWriteSettingPushButton = mDebugPage->mReadWriteSettingPushButton;
    autoResponseSettingPushButton = mDebugPage->mAutoResponseSettingPushButton;
    transmissionSettingPushButton = mDebugPage->mTransmissionSettingPushButton;

    connect(moreSettingsPushButton,  &QPushButton::clicked, this, &SAKDebugPageOtherController::onMoreSettingsPushButtonClicked);
    connect(timingSendingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onTimingSendingPushButtonClicked);
    connect(highlightSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onHighlightSettingPushButtonClicked);
    connect(readWriteSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onReadWriteSettingPushButtonClicked);
    connect(transmissionSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onTransmissionSettingPushButtonClicked);
    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKDebugPageOtherController::onAutoresponseSettingPushbuttonClicked);
}

SAKDebugPageOtherController::~SAKDebugPageOtherController()
{
    delete mMoreSettingsWidget;
    delete mTransmissionSettings;
    delete mHighlightSettingsWidget;
    delete mReadWriteSettingsWidget;
    delete mAutoResponseSettingWidget;
    delete mTimingSendingSettingsWidget;

    mMoreSettingsWidget = Q_NULLPTR;
    mTransmissionSettings = Q_NULLPTR;
    mHighlightSettingsWidget = Q_NULLPTR;
    mReadWriteSettingsWidget = Q_NULLPTR;
    mAutoResponseSettingWidget = Q_NULLPTR;
    mTimingSendingSettingsWidget = Q_NULLPTR;
}

SAKMoreSettingsWidget *SAKDebugPageOtherController::moreSettingsWidget()
{
    return mMoreSettingsWidget;
}

void SAKDebugPageOtherController::onMoreSettingsPushButtonClicked()
{
    if (mMoreSettingsWidget->isHidden()){
        mMoreSettingsWidget->show();
    }else {
        mMoreSettingsWidget->activateWindow();
    }
}

void SAKDebugPageOtherController::onTimingSendingPushButtonClicked()
{
    if (mTimingSendingSettingsWidget->isHidden()){
        mTimingSendingSettingsWidget->show();
    }else{
        mTimingSendingSettingsWidget->activateWindow();
    }
}

void SAKDebugPageOtherController::onReadWriteSettingPushButtonClicked()
{
    if (mReadWriteSettingsWidget->isHidden()){
        mReadWriteSettingsWidget->show();
    }else {
        mReadWriteSettingsWidget->activateWindow();
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
