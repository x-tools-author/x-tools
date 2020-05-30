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
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTcpServerDeviceController.hh"

SAKTcpServerDebugPage::SAKTcpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKTcpServerDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeTCPServer));
}

SAKTcpServerDebugPage::~SAKTcpServerDebugPage()
{
    tcpServerDeviceController->deleteLater();
}

SAKTcpServerDeviceController *SAKTcpServerDebugPage::controllerInstance()
{
    return tcpServerDeviceController;
}

void SAKTcpServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKTcpServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}

void SAKTcpServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    refreshPushButton->setEnabled(enable);
}

SAKDevice* SAKTcpServerDebugPage::createDevice()
{
    SAKTcpServerDevice *device = new SAKTcpServerDevice(this);
    return device;
}
