/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKDebuggerTextOutput.hh"
#include "SAKDebuggerOutputManager.hh"
#include "SAKDebuggerOutputSettings.hh"
#include "SAKDebuggerOutputTextFactory.hh"

SAKDebuggerOutputManager::SAKDebuggerOutputManager(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
{
    _outputSettings = new SAKDebuggerOutputSettings(debugger);
    _textOutput = new SAKDebuggerTextOutput(debugger);
    _textOutput->textFactoryInstance()->setOutputSettings(_outputSettings);
}

SAKDebuggerOutputManager::~SAKDebuggerOutputManager()
{
    delete _outputSettings;
    delete _textOutput;
}

SAKDebuggerOutputSettings *SAKDebuggerOutputManager::outputSettingsInstance()
{
    return _outputSettings;
}

SAKDebuggerTextOutput *SAKDebuggerOutputManager::textOutputInstance()
{
    return _textOutput;
}

SAKDebuggerOutputSettings *SAKDebuggerOutputManager::outputSettings()
{
    return _outputSettings;
}

SAKDebuggerTextOutput *SAKDebuggerOutputManager::textOutput()
{
    return _textOutput;
}
