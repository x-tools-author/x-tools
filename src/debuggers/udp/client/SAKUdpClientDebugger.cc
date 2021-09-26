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

#include "SAKUdpClientDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKUdpClientDebugger.hh"
#include "SAKUdpClientController.hh"

SAKUdpClientDebugger::SAKUdpClientDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKUdpClientController(settings, settingsGroup, this);
    mDevice = new SAKUdpClientDevice(settings, settingsGroup, parent, this);
    initDebugger();

    connect(mDevice, &SAKUdpClientDevice::clientInfoChanged,
            mController, &SAKUdpClientController::onClientInfoChanged);
}

SAKDebuggerDevice* SAKUdpClientDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKUdpClientDebugger::controller()
{
    return mController;
}
