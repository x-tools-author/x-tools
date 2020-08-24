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
#include "SAKCommonDataStructure.hh"
#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDebugPage::SAKTcpClientDebugPage(QWidget *parent)
    :SAKDebugPage(SAKCommonDataStructure::DebugPageTypeTCPClient, parent)
    ,mTcpClientDeviceController(Q_NULLPTR)
{
    mTcpClientDeviceController = new SAKTcpClientDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKCommonDataStructure::DebugPageTypeTCPClient));
    initializingPage();
}

SAKDebugPageController *SAKTcpClientDebugPage::deviceController()
{
    return mTcpClientDeviceController;
}

SAKDebugPageDevice *SAKTcpClientDebugPage::createDevice()
{
    auto ret = new SAKTcpClientDevice(this);
    return ret;
}
