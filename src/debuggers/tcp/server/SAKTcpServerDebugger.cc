/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugger.hh"
#include "SAKTcpServerController.hh"

SAKTcpServerDebugger::SAKTcpServerDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKTcpServerController(settings, settingsGroup, parent);
    mDevice = new SAKTcpServerDevice(settings, settingsGroup, this, parent);
    initDebugger();

    connect(mDevice, &SAKTcpServerDevice::addClient,
            mController, &SAKTcpServerController::onAddClient);
    connect(mDevice, &SAKTcpServerDevice::removeClient,
            mController, &SAKTcpServerController::onRemoveClient,
            Qt::QueuedConnection);
}

SAKDebuggerDevice* SAKTcpServerDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKTcpServerDebugger::controller()
{
    return mController;
}
