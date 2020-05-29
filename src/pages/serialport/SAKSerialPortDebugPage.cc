/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
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
#include "SAKDataStruct.hh"
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKSerialPortDeviceController.hh"

SAKSerialPortDebugPage::SAKSerialPortDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeCOM, parent)
    ,controller (new SAKSerialPortDeviceController)
{
    initPage();
    setWindowTitle(SAKGlobal::getNameOfDebugPage(SAKDataStruct::DebugPageTypeCOM));
}

SAKSerialPortDebugPage::~SAKSerialPortDebugPage()
{
    delete controller;
}

SAKSerialPortDeviceController *SAKSerialPortDebugPage::controllerInstance()
{
    return controller;
}

SAKDevice *SAKSerialPortDebugPage::createDevice()
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
    refreshPushButton->setEnabled(enable);
}
