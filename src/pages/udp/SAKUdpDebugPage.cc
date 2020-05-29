/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeUDP));
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

SAKDevice *SAKUdpDebugPage::createDevice()
{
    SAKUdpDevice *ptr = new SAKUdpDevice(this);
    udpDeviceController->setUdpDevice(ptr);
    return ptr;
}

void SAKUdpDebugPage::setUiEnable(bool enable)
{
    udpDeviceController->setUiEnable(enable);
    refreshPushButton->setEnabled(enable);
}
