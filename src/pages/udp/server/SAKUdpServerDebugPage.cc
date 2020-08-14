/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKGlobal.hh"
#include "SAKDataStruct.hh"
#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugPage.hh"
#include "SAKUdpServerDeviceController.hh"

SAKUdpServerDebugPage::SAKUdpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeUdpServer, parent)
{
    mDeviceController = new SAKUdpServerDeviceController(this);
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeUdpServer));
}

SAKUdpServerDebugPage::~SAKUdpServerDebugPage()
{
    mDeviceController->deleteLater();
}

SAKUdpServerDeviceController *SAKUdpServerDebugPage::controllerInstance()
{
    return mDeviceController;
}

SAKDebugPageController *SAKUdpServerDebugPage::deviceController()
{
    return mDeviceController;
}

SAKDebugPageDevice* SAKUdpServerDebugPage::device()
{
    SAKUdpServerDevice *device = new SAKUdpServerDevice(this);
    return device;
}
