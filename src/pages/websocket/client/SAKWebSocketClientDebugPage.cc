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
#include "SAKWebSocketClientDevice.hh"
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketClientDeviceController.hh"

SAKWebSocketClientDebugPage::SAKWebSocketClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPClient, parent)
    ,webSocketClientDeviceController (new SAKWebSocketClientDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPClient));
}

SAKWebSocketClientDebugPage::~SAKWebSocketClientDebugPage()
{
    webSocketClientDeviceController->deleteLater();
}

SAKWebSocketClientDeviceController *SAKWebSocketClientDebugPage::controllerInstance()
{
    return webSocketClientDeviceController;
}

void SAKWebSocketClientDebugPage::refreshDevice()
{
    /// @brief Nothing to do.
}

QWidget *SAKWebSocketClientDebugPage::controllerWidget()
{
    return webSocketClientDeviceController;
}

SAKDevice *SAKWebSocketClientDebugPage::createDevice()
{
    SAKWebSocketClientDevice *device = new SAKWebSocketClientDevice(this);
    return device;
}

void SAKWebSocketClientDebugPage::setUiEnable(bool enable)
{
    webSocketClientDeviceController->setEnabled(enable);
    mRefreshPushButton->setEnabled(enable);
}
