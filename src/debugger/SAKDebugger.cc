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
#include "SAKDebuggerInputManager.hh"
#include "SAKDebuggerOutputManager.hh"
#include "SAKDebuggerDeviceSerialport.hh"

SAKDebugger::SAKDebugger(int type, QObject *parent)
    :QObject (parent)
    ,debuggerType(type)
    ,crcInterface (new SAKCRCInterface)
    ,_inputManager (new SAKDebuggerInputManager)
    ,_outputManager (new SAKDebuggerOutputManager)
{
    if (!parent){
        setParent(qApp);
    }

    if (type == DebuggerTypeSerialport){
        _device = new SAKDebuggerDeviceSerialport(this);
    }

    _device->start();
}

SAKDebugger::~SAKDebugger()
{
    delete crcInterface;
    delete _inputManager;
    delete _outputManager;

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
