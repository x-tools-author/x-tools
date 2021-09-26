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

#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugger.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKUdpServerController.hh"

SAKUdpServerDebugger::SAKUdpServerDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKUdpServerController(settings, settingsGroup, parent);
    mDevice = new SAKUdpServerDevice(settings, settingsGroup, this, parent);
    initDebugger();

    connect(mDevice, &SAKUdpServerDevice::addClient,
            mController, &SAKUdpServerController::onAddClient);
}

SAKDebuggerDevice* SAKUdpServerDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKUdpServerDebugger::controller()
{
    return mController;
}
