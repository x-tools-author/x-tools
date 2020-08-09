/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKTestDevice.hh"
#include "SAKTestDebugPage.hh"
#include "SAKTestDeviceController.hh"

SAKTestDevice::SAKTestDevice(SAKTestDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,mDebugPage(debugPage)
{

}

SAKTestDevice::~SAKTestDevice()
{

}

void SAKTestDevice::run()
{
    QEventLoop eventLoop;
    exec();
}
