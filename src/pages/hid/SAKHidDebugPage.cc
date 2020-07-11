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
#include "SAKHidDevice.hh"
#include "SAKDataStruct.hh"
#include "SAKHidDebugPage.hh"
#include "SAKHidDeviceController.hh"

SAKHidDebugPage::SAKHidDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeHID, parent)
    ,controller (new SAKHidDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeHID));
}

SAKHidDebugPage::~SAKHidDebugPage()
{
    controller->deleteLater();
}

SAKHidDeviceController *SAKHidDebugPage::controllerInstance()
{
    return controller;
}

void SAKHidDebugPage::refreshDevice()
{
    controller->refresh();
}

QWidget *SAKHidDebugPage::controllerWidget()
{
    return controller;
}

SAKDevice* SAKHidDebugPage::createDevice()
{
    auto ret = new SAKHidDevice(this);
    return ret;
}

void SAKHidDebugPage::setUiEnable(bool enable)
{
    controller->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}
