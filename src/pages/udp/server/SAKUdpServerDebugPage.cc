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
#include "SAKCommonDataStructure.hh"
#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugPage.hh"
#include "SAKUdpServerDeviceController.hh"

SAKUdpServerDebugPage::SAKUdpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKCommonDataStructure::DebugPageTypeUdpServer, parent)
{
    mDeviceController = new SAKUdpServerDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKCommonDataStructure::DebugPageTypeUdpServer));
    initializingPage();
}

SAKUdpServerDebugPage::~SAKUdpServerDebugPage()
{
    mDeviceController->deleteLater();
}

SAKDebugPageController *SAKUdpServerDebugPage::deviceController()
{
    return mDeviceController;
}

SAKDebugPageDevice* SAKUdpServerDebugPage::createDevice()
{
    SAKUdpServerDevice *device = new SAKUdpServerDevice(this);
    return device;
}
