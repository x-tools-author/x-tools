/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketServerDevice.hh"
#include "SAKWebSocketServerDebugger.hh"
#include "SAKWebSocketServerController.hh"

SAKWebSocketServerDebugger::SAKWebSocketServerDebugger(QSettings *settings,
                                                       const QString settingsGroup,
                                                       QSqlDatabase *sqlDatabase,
                                                       QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKWebSocketServerController(settings, settingsGroup, parent);
    mDevice = new SAKWebSocketServerDevice(settings, settingsGroup, this, parent);
    initDebugger();
    connect(mDevice, &SAKWebSocketServerDevice::addClient,
            mController, &SAKWebSocketServerController::addClient);
    connect(mDevice, &SAKWebSocketServerDevice::removeClient,
            mController, &SAKWebSocketServerController::removeClient);
    connect(mDevice, &SAKWebSocketServerDevice::clearClient,
            mController, &SAKWebSocketServerController::clearClient);
}

SAKDebuggerDevice* SAKWebSocketServerDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKWebSocketServerDebugger::controller()
{
    return mController;
}
