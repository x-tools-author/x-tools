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
#include "SAKTestDevice.hh"
#include "SAKTestDebugPage.hh"
#include "SAKTestDeviceController.hh"

SAKTestDebugPage::SAKTestDebugPage(QWidget *parent)
    :SAKDebugPage (SAKCommonDataStructure::DebugPageTypeTest, parent)
    ,mDeviceController(Q_NULLPTR)
{
    mDeviceController = new SAKTestDeviceController(this);
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKCommonDataStructure::DebugPageTypeTest));
    initializingPage();
}

SAKDebugPageController *SAKTestDebugPage::deviceController()
{
    return mDeviceController;
}

SAKDebugPageDevice *SAKTestDebugPage::createDevice()
{
    auto ret = new SAKTestDevice(this);
    return ret;
}
