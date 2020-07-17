/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKBluetoothClientDevice.hh"
#include "SAKBluetoothClientDebugPage.hh"
#include "SAKBluetoothClientDeviceController.hh"

SAKBluetoothClientDebugPage::SAKBluetoothClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPClient, parent)
    ,tcpClientDeviceController (new SAKBluetoothClientDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPClient));
}

SAKBluetoothClientDebugPage::~SAKBluetoothClientDebugPage()
{
    tcpClientDeviceController->deleteLater();
}

SAKBluetoothClientDeviceController *SAKBluetoothClientDebugPage::controllerInstance()
{
    return tcpClientDeviceController;
}

void SAKBluetoothClientDebugPage::refreshDevice()
{
    tcpClientDeviceController->refresh();
}

QWidget *SAKBluetoothClientDebugPage::controllerWidget()
{
    return tcpClientDeviceController;
}

SAKDevice *SAKBluetoothClientDebugPage::createDevice()
{
    SAKBluetoothClientDevice *device = new SAKBluetoothClientDevice(this);
    return device;
}

void SAKBluetoothClientDebugPage::setUiEnable(bool enable)
{
    tcpClientDeviceController->setEnabled(enable);
    mRefreshPushButton->setEnabled(enable);
}
