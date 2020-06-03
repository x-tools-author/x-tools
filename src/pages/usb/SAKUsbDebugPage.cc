/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
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
#include "SAKUsbDevice.hh"
#include "SAKDataStruct.hh"
#include "SAKUsbDebugPage.hh"
#include "SAKUsbDeviceController.hh"

SAKUsbDebugPage::SAKUsbDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeUSB, parent)
    ,controller (new SAKUsbDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeUSB));
}

SAKUsbDebugPage::~SAKUsbDebugPage()
{
    controller->deleteLater();
}

SAKUsbDeviceController *SAKUsbDebugPage::controllerInstance()
{
    return controller;
}

void SAKUsbDebugPage::refreshDevice()
{
    controller->refresh();
}

QWidget *SAKUsbDebugPage::controllerWidget()
{
    return controller;
}

SAKDevice* SAKUsbDebugPage::createDevice()
{
    auto ret = new SAKUsbDevice(this);
    return ret;
}

void SAKUsbDebugPage::setUiEnable(bool enable)
{
    controller->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}
