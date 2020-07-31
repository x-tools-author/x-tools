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
#include "SAKUdpDevice.hh"
#include "SAKDataStruct.hh"
#include "SAKUdpDebugPage.hh"
#include "SAKUdpDeviceController.hh"

SAKUdpDebugPage::SAKUdpDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeUDP, parent)
    ,udpDeviceController (new SAKUdpDeviceController)
{
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeUDP));
}

SAKUdpDebugPage::~SAKUdpDebugPage()
{
    udpDeviceController->deleteLater();
}

SAKUdpDeviceController *SAKUdpDebugPage::controllerInstance()
{
    return udpDeviceController;
}

void SAKUdpDebugPage::refreshDevice()
{
    udpDeviceController->refresh();
}

QWidget *SAKUdpDebugPage::controllerWidget()
{
    return udpDeviceController;
}

SAKDebugPageDevice *SAKUdpDebugPage::createDevice()
{
    SAKUdpDevice *ptr = new SAKUdpDevice(this);
    udpDeviceController->setUdpDevice(ptr);
    return ptr;
}

void SAKUdpDebugPage::setUiEnable(bool enable)
{
    udpDeviceController->setUiEnable(enable);
    mRefreshPushButton->setEnabled(enable);
}
