/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
    initializingPage();
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
