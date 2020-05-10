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
#include "SAKDebuggerTextOutput.hh"

SAKDebuggerTextOutput::SAKDebuggerTextOutput(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
{
    SAKDebuggerDevice *device = debugger->deviceInstance();
    connect(device, &SAKDebuggerDevice::bytesRead, this, &SAKDebuggerTextOutput::outputText, Qt::QueuedConnection);
}

SAKDebuggerTextOutput::~SAKDebuggerTextOutput()
{

}

void SAKDebuggerTextOutput::outputText(QByteArray text)
{
    emit outputTextRequest(QString(text.toHex(' ')).toUpper());
}
