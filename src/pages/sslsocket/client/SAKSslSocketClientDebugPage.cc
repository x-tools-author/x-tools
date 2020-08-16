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
#include "SAKSslSocketClientDevice.hh"
#include "SAKSslSocketClientDebugPage.hh"
#include "SAKSslSocketClientDeviceController.hh"

SAKSslSocketClientDebugPage::SAKSslSocketClientDebugPage(QWidget *parent)
    :SAKDebugPage(SAKDataStruct::DebugPageTypeTCPClient, parent)
    ,mTcpClientDeviceController(Q_NULLPTR)
{
    mTcpClientDeviceController = new SAKSslSocketClientDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPClient));
    initializingPage();
}

SAKDebugPageController *SAKSslSocketClientDebugPage::deviceController()
{
    return mTcpClientDeviceController;
}

SAKDebugPageDevice *SAKSslSocketClientDebugPage::createDevice()
{
    auto ret = new SAKSslSocketClientDevice(this);
    return ret;
}
