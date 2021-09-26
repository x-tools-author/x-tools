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

#include "SAKCommonDataStructure.hh"
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugger.hh"
#include "SAKSerialPortController.hh"

SAKSerialPortDebugger::SAKSerialPortDebugger(QSettings *settings,
                                             const QString settingsGroup,
                                             QSqlDatabase *sqlDatabase,
                                             QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKSerialPortController(settings, settingsGroup, this);
    mDevice = new SAKSerialPortDevice(settings, settingsGroup, parent, this);
    initDebugger();
}

SAKDebuggerDevice *SAKSerialPortDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKSerialPortDebugger::controller()
{
    return mController;
}
