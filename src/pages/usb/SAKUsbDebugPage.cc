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
