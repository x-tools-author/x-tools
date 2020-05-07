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
#include <QGuiApplication>

#include "SAKDebugger.hh"
#include "SAKDebuggerDeviceSerialport.hh"

SAKDebugger::SAKDebugger(int type, QObject *parent)
    :QObject (parent)
{
    if (!parent){
        setParent(qApp);
    }

    if (type == DebuggerTypeSerialport){
        device = new SAKDebuggerDeviceSerialport(this);
    }
}

SAKDebugger::~SAKDebugger()
{
    device->requestInterruption();
    device->quit();
    device->wait();
}

SAKDebugger *SAKDebugger::createDebugger(int type)
{
    return new SAKDebugger(type);
}
