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
#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugger.hh"
#include "SAKUdpServerController.hh"

SAKUdpServerDebugger::SAKUdpServerDebugger(int type, QString name, QWidget *parent)
    :SAKDebugger(type, name, parent)
{
    mDeviceController = new SAKUdpServerController(this);
    mDevice = new SAKUdpServerDevice(this, this);
    initializePage();
}

SAKDebuggerDevice* SAKUdpServerDebugger::device()
{
    return mDevice;
}

SAKDebugPageController *SAKUdpServerDebugger::controller()
{
    return mDeviceController;
}
