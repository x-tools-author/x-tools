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
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKUdpServerDeviceController)
{
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
}

SAKUdpServerDebugPage::~SAKUdpServerDebugPage()
{
    tcpServerDeviceController->deleteLater();
}

SAKUdpServerDeviceController *SAKUdpServerDebugPage::controllerInstance()
{
    return tcpServerDeviceController;
}

void SAKUdpServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKUdpServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}

void SAKUdpServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    mRefreshPushButton->setEnabled(enable);
}

SAKDebugPageDevice* SAKUdpServerDebugPage::createDevice()
{
    SAKUdpServerDevice *device = new SAKUdpServerDevice(this);
    return device;
}
