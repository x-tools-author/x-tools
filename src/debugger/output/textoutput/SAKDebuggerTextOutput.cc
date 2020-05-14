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
#include "SAKDebuggerOutputManager.hh"
#include "SAKDebuggerOutputSettings.hh"
#include "SAKDebuggerOutputTextFactory.hh"

SAKDebuggerTextOutput::SAKDebuggerTextOutput(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
{
    textFactory = new SAKDebuggerOutputTextFactory(debugger);
    connect(textFactory, &SAKDebuggerOutputTextFactory::dataCooked, this, &SAKDebuggerTextOutput::dataCooked);
    textFactory->start();

    SAKDebuggerDevice *device = debugger->deviceInstance();
    connect(device, &SAKDebuggerDevice::bytesRead, this, &SAKDebuggerTextOutput::outputTextRx, Qt::QueuedConnection);
    connect(device, &SAKDebuggerDevice::bytesWritten, this, &SAKDebuggerTextOutput::outputTextTx, Qt::QueuedConnection);
    outputSettings = debugger->outputManagerInstance()->outputSettingsInstance();
}

SAKDebuggerTextOutput::~SAKDebuggerTextOutput()
{
    textFactory->requestInterruption();
    textFactory->quit();
    textFactory->wait();
    delete textFactory;
}

void SAKDebuggerTextOutput::outputText(QByteArray text, bool isRxData)
{
    textFactory->addRawData(text, isRxData);
    /// @brief 唤醒处理线程处理数据
    textFactory->wakeMe();
}

void SAKDebuggerTextOutput::outputTextRx(QByteArray text)
{
    outputText(text, true);
}

void SAKDebuggerTextOutput::outputTextTx(QByteArray text)
{
    outputText(text, false);
}

void SAKDebuggerTextOutput::dataCooked(QString text)
{
    emit outputTextRequest(text);
}
