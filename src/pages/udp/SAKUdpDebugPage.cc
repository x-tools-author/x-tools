/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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

SAKDevice *SAKUdpDebugPage::createDevice()
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
