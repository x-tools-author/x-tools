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
#include "SAKTestDevice.hh"
#include "SAKTestDebugPage.hh"
#include "SAKTestDeviceController.hh"

SAKTestDebugPage::SAKTestDebugPage(QWidget *parent)
    :SAKDebugPage (SAKDataStruct::DebugPageTypeTest, parent)
    ,mDevice(Q_NULLPTR)
    ,mDeviceController(Q_NULLPTR)
{
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTest));
    initializingPage();
}

SAKDebugPageDevice *SAKTestDebugPage::device()
{
    if (!mDevice){
        mDevice = new SAKTestDevice(this);
    }

    return mDevice;
}

SAKDebugPageController *SAKTestDebugPage::deviceController()
{
    if (!mDeviceController){
        mDeviceController = new SAKTestDeviceController(this);
    }
    return mDeviceController;
}
