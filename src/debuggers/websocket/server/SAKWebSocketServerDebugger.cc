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

SAKWebSocketServerDebugger::SAKWebSocketServerDebugger(int type, QString name, QWidget *parent)
    :SAKDebugPage(type, name, parent)
{
    mDeviceController = new SAKWebSocketServerController(this);
    mDevice = new SAKWebSocketServerDevice(this, this);
    initializePage();
}
