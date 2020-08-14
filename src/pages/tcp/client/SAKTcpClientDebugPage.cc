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
#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDebugPage::SAKTcpClientDebugPage(QWidget *parent)
    :SAKDebugPage(SAKDataStruct::DebugPageTypeTCPClient, parent)
    ,mTcpClientDeviceController(Q_NULLPTR)
    ,mTcpClientDevice(Q_NULLPTR)
{
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTCPClient));
    initializingPage();
}

SAKDebugPageController *SAKTcpClientDebugPage::deviceController()
{
    if (!mTcpClientDeviceController){
        mTcpClientDeviceController = new SAKTcpClientDeviceController(this);
    }
    return mTcpClientDeviceController;
}

SAKDebugPageDevice *SAKTcpClientDebugPage::device()
{
    if (!mTcpClientDevice){
        mTcpClientDevice = new SAKTcpClientDevice(this);
    }

    return mTcpClientDevice;
}
