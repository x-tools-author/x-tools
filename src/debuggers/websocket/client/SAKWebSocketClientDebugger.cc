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
#include "SAKWebSocketClientDevice.hh"
#include "SAKWebSocketClientDebugger.hh"
#include "SAKWebSocketClientController.hh"

SAKWebSocketClientDebugger::SAKWebSocketClientDebugger(QSettings *settings,
                                                       const QString settingsGroup,
                                                       QSqlDatabase *sqlDatabase,
                                                       QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKWebSocketClientController(settings, settingsGroup, parent);
    mDevice = new SAKWebSocketClientDevice(settings, settingsGroup, this, parent);
    initDebugger();
    connect(mDevice, &SAKWebSocketClientDevice::clientInfoChanged,
            mController, &SAKWebSocketClientController::onClientInfoChanged);
}

SAKDebuggerDevice* SAKWebSocketClientDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKWebSocketClientDebugger::controller()
{
    return mController;
}
