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
#include "SAKDebuggerDeviceSerialport.hh"

SAKDebuggerDeviceSerialport::SAKDebuggerDeviceSerialport(SAKDebugger *debugger, QObject *parent)
    :SAKDebuggerDevice (debugger, parent)
    ,serialport (Q_NULLPTR)
{

}

SAKDebuggerDeviceSerialport::~SAKDebuggerDeviceSerialport()
{

}

void SAKDebuggerDeviceSerialport::run()
{
    serialport = new QSerialPort;
    while (1) {
        /// @brief “优雅地”地退出线程
        if (isInterruptionRequested()){
            break;
        }
    }

    delete serialport;
}
