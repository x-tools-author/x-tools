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
#include "SAKDataStruct.hh"
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTcpServerDeviceController.hh"

SAKTcpServerDebugPage::SAKTcpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKTcpServerDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
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
    mRefreshPushButton->setEnabled(enable);
}

SAKDebugPageDevice* SAKTcpServerDebugPage::createDevice()
{
    SAKTcpServerDevice *device = new SAKTcpServerDevice(this);
    return device;
}
