/*
 * Copyright (C) 2019-2020 wuuhii. All rights reserved.
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
#include "SAKHidDevice.hh"
#include "SAKDataStruct.hh"
#include "SAKHidDebugPage.hh"
#include "SAKHidDeviceController.hh"

SAKHidDebugPage::SAKHidDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeHID, parent)
    ,controller (new SAKHidDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeHID));
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
