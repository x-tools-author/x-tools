/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugger.hh"
#include "SAKTcpClientController.hh"

SAKTcpClientDebugger::SAKTcpClientDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKTcpClientController(settings, settingsGroup, parent);
    mDevice = new  SAKTcpClientDevice(settings, settingsGroup, this, this);
    initDebugger();

    connect(mDevice, &SAKTcpClientDevice::serverInfoChanged,
            mController, &SAKTcpClientController::onServerInfoChanged);
}

SAKDebuggerDevice* SAKTcpClientDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKTcpClientDebugger::controller()
{
    return mController;
}
