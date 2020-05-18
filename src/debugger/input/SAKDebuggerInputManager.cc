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
#include "SAKDebuggerTextInput.hh"
#include "SAKDebuggerInputManager.hh"
#include "SAKDebuggerInputSettings.hh"

SAKDebuggerInputManager::SAKDebuggerInputManager(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
{
    _inputSettings = new SAKDebuggerInputSettings(debugger);
    _textInput = new SAKDebuggerTextInput(debugger);

    _textInput->setInputSettings(_inputSettings);
}

SAKDebuggerInputManager::~SAKDebuggerInputManager()
{
    delete _inputSettings;
    delete _textInput;
}

SAKDebuggerInputSettings *SAKDebuggerInputManager::inputSettingsInstance()
{
    return _inputSettings;
}

SAKDebuggerTextInput *SAKDebuggerInputManager::textInputInstance()
{
    return _textInput;
}

SAKDebuggerInputSettings *SAKDebuggerInputManager::inputSettings()
{
    return _inputSettings;
}

SAKDebuggerTextInput* SAKDebuggerInputManager::textInput()
{
    return _textInput;
}

QString SAKDebuggerInputManager::currentString()
{
    return _currentString;
}

void SAKDebuggerInputManager::setCurrentString(QString str)
{
    _currentString = str;
    emit currentStringChanged();
}
