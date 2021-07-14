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

SAKTcpClientDebugger::SAKTcpClientDebugger(int type, QString name, QWidget *parent)
    :SAKDebugger(type, name, parent)
{
    mDeviceController = new SAKTcpClientController(this);
    mDevice = new  SAKTcpClientDevice(this, this);
    initializePage();
}
