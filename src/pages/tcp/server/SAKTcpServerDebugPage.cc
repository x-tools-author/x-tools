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
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTcpServerDeviceController.hh"

SAKTcpServerDebugPage::SAKTcpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTCPServer, parent)
    ,tcpServerDeviceController (new SAKTcpServerDeviceController)
{
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPServer));
}

SAKTcpServerDebugPage::~SAKTcpServerDebugPage()
{
    tcpServerDeviceController->deleteLater();
}

SAKTcpServerDeviceController *SAKTcpServerDebugPage::controllerInstance()
{
    return tcpServerDeviceController;
}

void SAKTcpServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKTcpServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}

void SAKTcpServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    mRefreshPushButton->setEnabled(enable);
}

SAKDebugPageDevice* SAKTcpServerDebugPage::createDevice()
{
    SAKTcpServerDevice *device = new SAKTcpServerDevice(this);
    return device;
}
