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
#include "SAKDataStruct.hh"
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpClientDeviceController.hh"

SAKUdpClientDebugPage::SAKUdpClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeUdpClient, parent)
{
    udpDeviceController = new SAKUdpClientDeviceController(this);
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeUdpClient));
}

SAKUdpClientDebugPage::~SAKUdpClientDebugPage()
{
    udpDeviceController->deleteLater();
}

SAKUdpClientDeviceController *SAKUdpClientDebugPage::controllerInstance()
{
    return udpDeviceController;
}

SAKDebugPageController *SAKUdpClientDebugPage::deviceController()
{
    return udpDeviceController;
}

SAKDebugPageDevice *SAKUdpClientDebugPage::device()
{
    SAKUdpClientDevice *ptr = new SAKUdpClientDevice(this);
    udpDeviceController->setUdpDevice(ptr);
    return ptr;
}
