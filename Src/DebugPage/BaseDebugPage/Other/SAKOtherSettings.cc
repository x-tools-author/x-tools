/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QDebug>

#include "SAKDebugPage.hh"
#include "ui_SAKDebugPage.h"
#include "SAKOtherSettings.hh"
#include "TransmissionSettings.hh"
#include "FormatSettingsWidget.hh"
#include "HighlightSettingsWidget.hh"
#include "MoreOtherSettingsWidget.hh"
#include "ReadWriteSettingsWidget.hh"
#include "SAKTimingSendingManager.hh"
#include "AutoResponseSettingWidget.hh"

SAKOtherSettings::SAKOtherSettings(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
{
    transmissionSettings        = new TransmissionSettings(_debugPage);
    autoResponseSettingWidget   = new AutoResponseSettingWidget(_debugPage);
    highlighterSettingPanel     = new HighlightSettingsWidget(_debugPage->ui->outputTextBroswer->document());
    moreOtherSettingsWidget     = new MoreOtherSettingsWidget(_debugPage);
    formatSettingsWidget        = new FormatSettingsWidget(_debugPage);
    readWriteSettingsWidget     = new ReadWriteSettingsWidget(_debugPage);
    timingSendingManager        = new SAKTimingSendingManager(_debugPage);

    autoResponseSettingPushButton   = _debugPage->ui->autoResponseSettingPushButton;
    highlightSettingPushButton      = _debugPage->ui->highlightSettingPushButton;
    readWriteSettingPushButton      = _debugPage->ui->readWriteSettingPushButton;
    transmissionSettingPushButton   = _debugPage->ui->transmissionSettingPushButton;
    moreOtherSettingsPushButton     = _debugPage->ui->moreOhterSettingsPushButton;    
    formatSettingsPushButton        = _debugPage->ui->formatSettingsPushButton;
    timingSendPushButton            = _debugPage->ui->timingSendPushButton;

    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKOtherSettings::onAutoresponseSettingPushbuttonClicked);
    connect(highlightSettingPushButton,    &QPushButton::clicked, this, &SAKOtherSettings::onHighlightSettingPushButtonClicked);
    connect(readWriteSettingPushButton,    &QPushButton::clicked, this, &SAKOtherSettings::onReadWriteSettingPushButtonClicked);
    connect(transmissionSettingPushButton, &QPushButton::clicked, this, &SAKOtherSettings::onTransmissionSettingPushButtonClicked);
    connect(moreOtherSettingsPushButton,   &QPushButton::clicked, this, &SAKOtherSettings::onMoreOtherSettingsPushButtonClicked);
    connect(formatSettingsPushButton,      &QPushButton::clicked, this, &SAKOtherSettings::onFormatSettingsPushButtonClicked);
    connect(timingSendPushButton,          &QPushButton::clicked, this, &SAKOtherSettings::onTimingSendPushButtonClicked);
}

SAKOtherSettings::~SAKOtherSettings()
{
    delete transmissionSettings;
    delete autoResponseSettingWidget;

    transmissionSettings = nullptr;
    autoResponseSettingWidget = nullptr;
}

void SAKOtherSettings::onAutoresponseSettingPushbuttonClicked()
{
    if (autoResponseSettingWidget->isHidden()){
        autoResponseSettingWidget->show();
    }else {
        autoResponseSettingWidget->activateWindow();
    }
}

void SAKOtherSettings::onHighlightSettingPushButtonClicked()
{

    if (highlighterSettingPanel->isHidden()){
        highlighterSettingPanel->show();
    }else {
        highlighterSettingPanel->activateWindow();
    }
}

void SAKOtherSettings::onReadWriteSettingPushButtonClicked()
{
    if (readWriteSettingsWidget->isHidden()){
        readWriteSettingsWidget->show();
    }else {
        readWriteSettingsWidget->activateWindow();
    }
}

void SAKOtherSettings::onTransmissionSettingPushButtonClicked()
{
    if (transmissionSettings->isHidden()){
        transmissionSettings->show();
    }else {
        transmissionSettings->activateWindow();
    }
}

void SAKOtherSettings::onMoreOtherSettingsPushButtonClicked()
{
    if (moreOtherSettingsWidget->isHidden()){
        moreOtherSettingsWidget->show();
    }else {
        moreOtherSettingsWidget->activateWindow();
    }
}

void SAKOtherSettings::onFormatSettingsPushButtonClicked()
{
    if (formatSettingsWidget->isHidden()){
        formatSettingsWidget->show();
    }else {
        formatSettingsWidget->activateWindow();
    }
}

void SAKOtherSettings::onTimingSendPushButtonClicked()
{
    if (timingSendingManager->isHidden()){
        timingSendingManager->show();
    }else{
        timingSendingManager->activateWindow();
    }
}
