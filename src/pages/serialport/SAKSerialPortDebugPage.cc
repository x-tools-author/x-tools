/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKGlobal.hh"
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKSerialPortDeviceController.hh"

SAKSerialPortDebugPage::SAKSerialPortDebugPage(QWidget *parent)
    :SAKDebugPage (SAKGlobal::SAKEnumDebugPageTypeCOM, parent)
    ,serialPortAssistant (Q_NULLPTR)
    ,controller (new SAKSerialPortDeviceController)
{
    initPage();
    setWindowTitle(tr("串口调试"));
}

SAKSerialPortDebugPage::~SAKSerialPortDebugPage()
{
    delete controller;
}

SAKDevice *SAKSerialPortDebugPage::createDevice()
{
    serialPortAssistant = new SAKSerialPortDevice;
    return serialPortAssistant;
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
