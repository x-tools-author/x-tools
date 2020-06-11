/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
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
#include "SAKWebSocketServerDevice.hh"
#include "SAKWebSocketServerDebugPage.hh"
#include "SAKWebSocketServerDeviceController.hh"

SAKWebSocketServerDebugPage::SAKWebSocketServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKWebSocketServerDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeTCPServer));
}

SAKWebSocketServerDebugPage::~SAKWebSocketServerDebugPage()
{
    tcpServerDeviceController->deleteLater();
}

SAKWebSocketServerDeviceController *SAKWebSocketServerDebugPage::controllerInstance()
{
    return tcpServerDeviceController;
}

void SAKWebSocketServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKWebSocketServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}

void SAKWebSocketServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    refreshPushButton->setEnabled(enable);
}

SAKDevice* SAKWebSocketServerDebugPage::createDevice()
{
    SAKWebSocketServerDevice *device = new SAKWebSocketServerDevice(this);
    return device;
}
