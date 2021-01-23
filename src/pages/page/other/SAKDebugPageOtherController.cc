/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMenu>
#include <QDebug>
#include <QAction>

#include "SAKDebugPage.hh"
#include "SAKOtherTransmissionPageViewer.hh"
#include "SAKOtherHighlighterManager.hh"
#include "SAKDebugPageOtherController.hh"
#include "SAKOtherAutoResponseItemManager.hh"
#include "SAKOtherTimingSentItemManager.hh"
#include "SAKOtherAnalyzerThreadManager.hh"

SAKDebugPageOtherController::SAKDebugPageOtherController(SAKDebugPage *debugPage, QObject *parent)
    :QObject(parent)
    ,mDebugPage(debugPage)
{
    mTransmissionSettings = new SAKOtherTransmissionPageViewer(mDebugPage);
    mHighlightSettingsWidget = new SAKOtherHighlighterManager(mDebugPage->mOutputTextBroswer->document());
    mAutoResponseSettingWidget = new SAKOtherAutoResponseItemManager(mDebugPage);
    mTimingSendingSettingsWidget = new SAKOtherTimingSentItemManager(mDebugPage);
    mAnalyzerThreadManager = new SAKOtherAnalyzerThreadManager(debugPage->settings());

    moreSettingsPushButton = mDebugPage->mMoreSettingsPushButton;
    timingSendingPushButton = mDebugPage->mTimingSendingPushButton;
    autoResponseSettingPushButton = mDebugPage->mAutoResponseSettingPushButton;
    transmissionSettingPushButton = mDebugPage->mTransmissionSettingPushButton;
    mAnalyzerPushButton = mDebugPage->mAnalyzerPushButton;

    auto moreSettingsPushButtonMenu = new QMenu;
    connect(this, &SAKDebugPageOtherController::destroyed, moreSettingsPushButtonMenu, &QMenu::deleteLater);
    moreSettingsPushButton->setMenu(moreSettingsPushButtonMenu);
    moreSettingsPushButton->setEnabled(false);
#if 0
    auto highlightSettingsAction = new QAction(tr("Highlight Settings"), this);
    moreSettingsPushButtonMenu->addAction(highlightSettingsAction);
    connect(highlightSettingsAction, &QAction::triggered, this, [=](){
        if (mHighlightSettingsWidget->isHidden()){
            mHighlightSettingsWidget->show();
        }else {
            mHighlightSettingsWidget->activateWindow();
        }
    });
#endif

    connect(timingSendingPushButton, &QPushButton::clicked, this, [=](){
        if (mTimingSendingSettingsWidget->isHidden()){
            mTimingSendingSettingsWidget->show();
        }else{
            mTimingSendingSettingsWidget->activateWindow();
        }
    });

    connect(transmissionSettingPushButton, &QPushButton::clicked, this, [=](){
        if (mTransmissionSettings->isHidden()){
            mTransmissionSettings->show();
        }else {
            mTransmissionSettings->activateWindow();
        }
    });

    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, [=](){
        if (mAutoResponseSettingWidget->isHidden()){
            mAutoResponseSettingWidget->show();
        }else {
            mAutoResponseSettingWidget->activateWindow();
        }
    });

    connect(mAnalyzerPushButton, &QPushButton::clicked, this, [=](){
        mAnalyzerThreadManager->show();
    });
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
