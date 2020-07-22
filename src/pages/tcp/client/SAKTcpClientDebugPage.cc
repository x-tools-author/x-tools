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
#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDebugPage::SAKTcpClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPClient, parent)
    ,tcpClientDeviceController (new SAKTcpClientDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPClient));
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

SAKDebugPageDevice *SAKTcpClientDebugPage::createDevice()
{
    SAKTcpClientDevice *device = new SAKTcpClientDevice(this);
    return device;
}

void SAKTcpClientDebugPage::setUiEnable(bool enable)
{
    tcpClientDeviceController->setEnabled(enable);
    mRefreshPushButton->setEnabled(enable);
}
