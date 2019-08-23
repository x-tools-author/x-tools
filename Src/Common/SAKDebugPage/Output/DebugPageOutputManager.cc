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
#include "DebugPageOutputManager.hh"

DebugPageOutputManager::DebugPageOutputManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
{
    rxLabel                 = _debugPage->rxLabel;
    txLabel                 = _debugPage->txLabel;
    outputModelComboBox     = _debugPage->outputModelComboBox;
    showDateCheckBox        = _debugPage->showDateCheckBox;
    autoWrapCheckBox        = _debugPage->autoWrapCheckBox;
    showTimeCheckBox        = _debugPage->showTimeCheckBox;
    showMsCheckBox          = _debugPage->showMsCheckBox;
    showRxDataCheckBox      = _debugPage->showRxDataCheckBox;
    showTxDataCheckBox      = _debugPage->showTxDataCheckBox;
    clearOutputPushButton   = _debugPage->clearOutputPushButton;
    saveOutputPushButton    = _debugPage->saveOutputPushButton;
    outputTextBroswer       = _debugPage->outputTextBroswer;
}

