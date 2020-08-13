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
    ,mController (new SAKTestDeviceController)
{
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeTest));
}

SAKTestDebugPage::~SAKTestDebugPage()
{
    delete mController;
}

SAKTestDeviceController *SAKTestDebugPage::controller()
{
    return mController;
}

SAKDebugPageDevice *SAKTestDebugPage::createDevice()
{
    SAKTestDevice *ret = new SAKTestDevice(this);
    return ret;
}

void SAKTestDebugPage::refreshDevice()
{
    mController->refresh();
}

QWidget *SAKTestDebugPage::controllerWidget()
{
    return mController;
}

void SAKTestDebugPage::setUiEnable(bool enable)
{
    mController->setEnabled(enable);
    mRefreshPushButton->setEnabled(enable);
}
