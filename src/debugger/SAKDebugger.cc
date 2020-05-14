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
#include <QDebug>
#include <QMetaEnum>
#include <QGuiApplication>

#include "SAKDebugger.hh"
#include "SAKCRCInterface.hh"
#include "SAKDebuggerTextOutput.hh"
#include "SAKDebuggerInputManager.hh"
#include "SAKDebuggerOutputManager.hh"
#include "SAKDebuggerOutputSettings.hh"
#include "SAKDebuggerDeviceSerialport.hh"
#include "SAKDebuggerOutputStatistics.hh"

SAKDebugger::SAKDebugger(int type, QObject *parent)
    :QObject (parent)
    ,debuggerType(type)
    ,crcInterface (new SAKCRCInterface)
{
    /// @brief 注意初始化顺序，不能乱
    _inputManager = new SAKDebuggerInputManager(this);
    if (type == DebuggerTypeSerialport){
        _device = new SAKDebuggerDeviceSerialport(this);
    }
    _outputManager = new SAKDebuggerOutputManager(this);
    _outputManager->textOutputInstance()->setDevice(_device);
    _outputManager->outputSettingsInstance()->statisticsInstance()->setDevice(_device);

    /// @brief 未指定parent的，统一将parent设置为qApp
    if (!parent){
        setParent(qApp);
    }

    _device->start();
}

SAKDebugger::~SAKDebugger()
{
    delete crcInterface;
    delete _inputManager;
    delete _outputManager;

    _device->wakeMe();
    _device->requestInterruption();
    _device->quit();
    _device->wait();
}

void SAKDebugger::setMessage(QString msg, bool isError)
{
    messageMutex.lock();
    Q_UNUSED(isError);
    qDebug() << __FUNCTION__ << msg;
    messageMutex.unlock();
}

SAKDebuggerInputManager *SAKDebugger::inputManagerInstance()
{
    return _inputManager;
}

SAKDebuggerOutputManager *SAKDebugger::outputManagerInstance()
{
    return _outputManager;
}

SAKDebuggerDevice *SAKDebugger::deviceInstance()
{
    return _device;
}

SAKDebuggerDevice *SAKDebugger::device()
{
    return _device;
}

SAKDebuggerInputManager *SAKDebugger::inputManager()
{
    return _inputManager;
}

SAKDebuggerOutputManager *SAKDebugger::outputManager()
{
    return _outputManager;
}
