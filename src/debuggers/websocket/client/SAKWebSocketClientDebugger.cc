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

SAKWebSocketClientDebugger::SAKWebSocketClientDebugger(int type, QString name, QWidget *parent)
    :SAKDebugPage(type, name, parent)
{
    mDeviceController = new SAKWebSocketClientController(this, this);
    mDevice = new SAKWebSocketClientDevice(this, this);
    initializePage();
}
