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
#include "AutoResponseSettingWidget.hh"

SAKOtherSettings::SAKOtherSettings(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
{
    transmissionSettings = new TransmissionSettings(_debugPage);
    autoResponseSettingWidget = new AutoResponseSettingWidget(_debugPage);

    autoResponseSettingPushButton   = debugPage->ui->autoResponseSettingPushButton;
    highlightSettingPushButton      = debugPage->ui->highlightSettingPushButton;
    readWriteSettingPushButton      = debugPage->ui->readWriteSettingPushButton;
    transmissionSettingPushButton   = debugPage->ui->transmissionSettingPushButton;

    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKOtherSettings::onAutoresponseSettingPushbuttonClicked);
    connect(highlightSettingPushButton,    &QPushButton::clicked, this, &SAKOtherSettings::onHighlightSettingPushButtonClicked);
    connect(readWriteSettingPushButton,    &QPushButton::clicked, this, &SAKOtherSettings::onReadWriteSettingPushButtonClicked);
    connect(transmissionSettingPushButton, &QPushButton::clicked, this, &SAKOtherSettings::onTransmissionSettingPushButtonClicked);
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

}

void SAKOtherSettings::onReadWriteSettingPushButtonClicked()
{

}

void SAKOtherSettings::onTransmissionSettingPushButtonClicked()
{
    if (transmissionSettings->isHidden()){
        transmissionSettings->show();
    }else {
        transmissionSettings->activateWindow();
    }
}
