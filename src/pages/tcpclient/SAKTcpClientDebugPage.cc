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
#include "SAKDataStruct.hh"
#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDebugPage::SAKTcpClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPClient, parent)
    ,tcpClientDeviceController (new SAKTcpClientDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeTCPClient));
}

SAKTcpClientDebugPage::~SAKTcpClientDebugPage()
{
    tcpClientDeviceController->deleteLater();
}

SAKTcpClientDeviceController *SAKTcpClientDebugPage::controllerInstance()
{
    return tcpClientDeviceController;
}

void SAKTcpClientDebugPage::refreshDevice()
{
    tcpClientDeviceController->refresh();
}

QWidget *SAKTcpClientDebugPage::controllerWidget()
{
    return tcpClientDeviceController;
}

SAKDevice *SAKTcpClientDebugPage::createDevice()
{
    SAKTcpClientDevice *device = new SAKTcpClientDevice(this);
    return device;
}

void SAKTcpClientDebugPage::setUiEnable(bool enable)
{
    tcpClientDeviceController->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}
