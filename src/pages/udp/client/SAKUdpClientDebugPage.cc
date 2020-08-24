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
#include <QWidget>
#include <QHBoxLayout>

#include "SAKGlobal.hh"
#include "SAKUdpClientDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpClientDeviceController.hh"

SAKUdpClientDebugPage::SAKUdpClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKCommonDataStructure::DebugPageTypeUdpClient, parent)
{
    mUdpDeviceController = new SAKUdpClientDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKCommonDataStructure::DebugPageTypeUdpClient));
    initializingPage();
}

SAKUdpClientDebugPage::~SAKUdpClientDebugPage()
{
    mUdpDeviceController->deleteLater();
}

SAKDebugPageController *SAKUdpClientDebugPage::deviceController()
{
    return mUdpDeviceController;
}

SAKDebugPageDevice *SAKUdpClientDebugPage::createDevice()
{
    SAKUdpClientDevice *ptr = new SAKUdpClientDevice(this);
    mUdpDeviceController->setUdpDevice(ptr);
    return ptr;
}
