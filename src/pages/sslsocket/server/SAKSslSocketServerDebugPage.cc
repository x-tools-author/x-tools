/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKSslSocketServerDevice.hh"
#include "SAKSslSocketServerDebugPage.hh"
#include "SAKSslSocketServerDeviceController.hh"

SAKSslSocketServerDebugPage::SAKSslSocketServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
{
    mDeviceController = new SAKSslSocketServerDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
    initializingPage();
}

SAKDebugPageController *SAKSslSocketServerDebugPage::deviceController()
{
    return mDeviceController;
}

SAKDebugPageDevice* SAKSslSocketServerDebugPage::createDevice()
{
    SAKSslSocketServerDevice *device = new SAKSslSocketServerDevice(this);
    return device;
}
