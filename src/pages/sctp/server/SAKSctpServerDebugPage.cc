/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
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
#include "SAKSctpServerDevice.hh"
#include "SAKSctpServerDebugPage.hh"
#include "SAKSctpServerDeviceController.hh"

SAKSctpServerDebugPage::SAKSctpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKSctpServerDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
}

SAKSctpServerDebugPage::~SAKSctpServerDebugPage()
{
    tcpServerDeviceController->deleteLater();
}

SAKSctpServerDeviceController *SAKSctpServerDebugPage::controllerInstance()
{
    return tcpServerDeviceController;
}

void SAKSctpServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKSctpServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}

void SAKSctpServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    refreshPushButton->setEnabled(enable);
}

SAKDevice* SAKSctpServerDebugPage::createDevice()
{
    SAKSctpServerDevice *device = new SAKSctpServerDevice(this);
    return device;
}
