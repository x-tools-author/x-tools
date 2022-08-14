/******************************************************************************
 * Copyright 2022 wuhai(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKBleCentralDevice.hh"
#include "SAKBleCentralDebugger.hh"
#include "SAKBleCentralController.hh"

SAKBleCentralDebugger::SAKBleCentralDebugger(QSettings *settings,
                                             const QString settingsGroup,
                                             QSqlDatabase *sqlDatabase,
                                             QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKBleCentralController(settings, settingsGroup, this);
    mDevice = new SAKBleCentralDevice(settings, settingsGroup, parent, this);

    connect(mDevice, &SAKBleCentralDevice::servicesChanged,
            mController, &SAKBleCentralController::setServices);

    initDebugger();
}

SAKDebuggerDevice *SAKBleCentralDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKBleCentralDebugger::controller()
{
    return mController;
}
