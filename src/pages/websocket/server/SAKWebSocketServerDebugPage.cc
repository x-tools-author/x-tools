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
#include "SAKWebSocketServerDevice.hh"
#include "SAKWebSocketServerDebugPage.hh"
#include "SAKWebSocketServerDeviceController.hh"

SAKWebSocketServerDebugPage::SAKWebSocketServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
{
    mTcpServerDeviceController = new SAKWebSocketServerDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
    initializingPage();
}

SAKWebSocketServerDebugPage::~SAKWebSocketServerDebugPage()
{
    mTcpServerDeviceController->deleteLater();
}

SAKDebugPageController *SAKWebSocketServerDebugPage::deviceController()
{
    return mTcpServerDeviceController;
}

SAKDebugPageDevice* SAKWebSocketServerDebugPage::createDevice()
{
    SAKWebSocketServerDevice *device = new SAKWebSocketServerDevice(this);
    return device;
}
