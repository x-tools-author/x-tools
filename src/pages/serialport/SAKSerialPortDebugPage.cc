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
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKSerialPortDeviceController.hh"

SAKSerialPortDebugPage::SAKSerialPortDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeCOM, parent)
    ,controller (new SAKSerialPortDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeCOM));
}

SAKSerialPortDebugPage::~SAKSerialPortDebugPage()
{
    delete controller;
}

SAKSerialPortDeviceController *SAKSerialPortDebugPage::controllerInstance()
{
    return controller;
}

SAKDebugPageDevice *SAKSerialPortDebugPage::createDevice()
{
    SAKSerialPortDevice *ret = new SAKSerialPortDevice(this);
    return ret;
}

void SAKSerialPortDebugPage::refreshDevice()
{
    controller->refresh();
}

QWidget *SAKSerialPortDebugPage::controllerWidget()
{
    return controller;
}

void SAKSerialPortDebugPage::setUiEnable(bool enable)
{
    controller->setEnabled(enable);
    mRefreshPushButton->setEnabled(enable);
}
