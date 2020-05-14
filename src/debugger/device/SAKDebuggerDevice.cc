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
#include "SAKDebugger.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerTextInput.hh"
#include "SAKDebuggerInputManager.hh"

SAKDebuggerDevice::SAKDebuggerDevice(SAKDebugger *debugger, QObject *parent)
    :QThread (parent)
    ,openRequest (false)
    ,clostRequest (false)
    ,readInterval (10)
    ,debugger (debugger)
    ,_deviceIsOpened (false)
{
    connect(this, &SAKDebuggerDevice::deviceStateChanged, this, &SAKDebuggerDevice::setDeviceIsOpened);

    /// @brief 关联发送请求
    SAKDebuggerTextInput *textInput = debugger->inputManagerInstance()->textInputInstance();
    connect(textInput, &SAKDebuggerTextInput::writeBytesRequest, this, &SAKDebuggerDevice::writeBytes);
}

SAKDebuggerDevice::~SAKDebuggerDevice()
{

}

void SAKDebuggerDevice::wakeMe()
{
    deviceWaitCondition.wakeAll();
}

void SAKDebuggerDevice::writeBytes(QByteArray bytes)
{
    dataListMutex.lock();
    if (_deviceIsOpened){
        dataList.append(bytes);
    }
    dataListMutex.unlock();

    /// @brief 唤醒线程进行数据发送
    deviceWaitCondition.wakeAll();
}

QByteArray SAKDebuggerDevice::takeArrayFromDataList()
{
    QByteArray array;
    dataListMutex.lock();
    if (dataList.length()){
        array = dataList.takeFirst();
    }
    dataListMutex.unlock();
    return array;
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
