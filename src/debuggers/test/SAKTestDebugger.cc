/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKTestDebugger.hh"

SAKTestDebugger::SAKTestDebugger(QSettings *settings,
                                 const QString settingsGroup,
                                 QSqlDatabase *sqlDatabase,
                                 QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
    ,mDevice(Q_NULLPTR)
    ,mController(Q_NULLPTR)
{
    mController = new SAKTestDebuggerController(settings, settingsGroup);
    mDevice = new SAKTestDebuggerDevice(settings, settingsGroup, this, this);
    initDebugger();

    connect(mController, &SAKTestDebuggerController::openFailedChanged,
            mDevice, &SAKTestDebuggerDevice::onOpenFailedChanged);
    connect(mController, &SAKTestDebuggerController::errorStringChanged,
            mDevice, &SAKTestDebuggerDevice::onErrorStringChanged);

    connect(mController, &SAKTestDebuggerController::readCircularlyChanged,
            mDevice, [&](){
        generateReadData();
    });
    connect(mController, &SAKTestDebuggerController::readIntervalChanged,
            mDevice, [&](){
        generateReadData();
    });

    connect(mController, &SAKTestDebuggerController::writeCircularlyChanged,
            mDevice, [&](){
        generateWriteData();
    });
    connect(mController, &SAKTestDebuggerController::writtingIntervalChanged,
            mDevice, [&](){
        generateWriteData();
    });

    auto ctx = mController->parametersContext();
    emit mController->openFailedChanged(ctx.openFailed);
    emit mController->errorStringChanged(ctx.errorString);
}

SAKDebuggerDevice* SAKTestDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKTestDebugger::controller()
{
    return mController;
}

void SAKTestDebugger::generateReadData()
{
    auto ctx = mController->parametersContext();
    bool start = ctx.readCircularly;
    int interval = ctx.readInterval;
    mDevice->generateReadData(start, interval);
}

void SAKTestDebugger::generateWriteData()
{
    auto ctx = mController->parametersContext();
    bool start = ctx.writeCircularly;
    int interval = ctx.writtingInterval;
    mDevice->generateWriteData(start, interval);
}
