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
    :SAKDebugPage(SAKDataStruct::DebugPageTypeCOM, parent)

{
    mDeviceController = new SAKSerialPortDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeCOM));
    initializingPage();
}

SAKSerialPortDebugPage::~SAKSerialPortDebugPage()
{
    delete mDeviceController;
}

SAKDebugPageController *SAKSerialPortDebugPage::deviceController()
{
    return mDeviceController;
}

SAKDebugPageDevice *SAKSerialPortDebugPage::createDevice()
{
    // The ret will be destroied when closing device
    auto ret = new SAKSerialPortDevice(this);
    return ret;
}
