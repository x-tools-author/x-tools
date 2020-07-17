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
#include "SAKBluetoothServerDevice.hh"
#include "SAKBluetoothServerDebugPage.hh"
#include "SAKBluetoothServerDeviceController.hh"

SAKBluetoothServerDebugPage::SAKBluetoothServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKBluetoothServerDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
}

SAKBluetoothServerDebugPage::~SAKBluetoothServerDebugPage()
{
    tcpServerDeviceController->deleteLater();
}

SAKBluetoothServerDeviceController *SAKBluetoothServerDebugPage::controllerInstance()
{
    return tcpServerDeviceController;
}

void SAKBluetoothServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKBluetoothServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}

void SAKBluetoothServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    mRefreshPushButton->setEnabled(enable);
}

SAKDevice* SAKBluetoothServerDebugPage::createDevice()
{
    SAKBluetoothServerDevice *device = new SAKBluetoothServerDevice(this);
    return device;
}
