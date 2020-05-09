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
#include "SAKDebuggerDevice.hh"

SAKDebuggerDevice::SAKDebuggerDevice(SAKDebugger *debugger, QObject *parent)
    :QThread (parent)
    ,openRequest (false)
    ,clostRequest (false)
    ,readInterval (10)
    ,debugger (debugger)
    ,_deviceIsOpened (false)
{
    connect(this, &SAKDebuggerDevice::deviceStateChanged, this, &SAKDebuggerDevice::setDeviceIsOpened);
}

SAKDebuggerDevice::~SAKDebuggerDevice()
{

}

bool SAKDebuggerDevice::deviceIsOpened()
{
    return _deviceIsOpened;
}

void SAKDebuggerDevice::setDeviceIsOpened(bool opened)
{
    _deviceIsOpened = opened;
    emit deviceIsOpenedChanged();
}
