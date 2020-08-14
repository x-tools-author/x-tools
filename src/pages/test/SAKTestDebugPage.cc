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
{
    mDeviceController = new SAKTestDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTest));
    initializingPage();
}

SAKTestDebugPage::~SAKTestDebugPage()
{
    delete mDeviceController;
}

SAKDebugPageDevice *SAKTestDebugPage::device()
{
    SAKTestDevice *ret = new SAKTestDevice(this);
    return ret;
}

SAKDebugPageController *SAKTestDebugPage::deviceController()
{
    return mDeviceController;
}
