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
    :SAKDebugPage (SAKDataStruct::DebugPageTypeCOM, parent)
    ,controller (new SAKTestDeviceController)
{
    initializingPage();
    setWindowTitle(SAKGlobal::debugPageNameFromType(SAKDataStruct::DebugPageTypeCOM));
}

SAKTestDebugPage::~SAKTestDebugPage()
{
    delete controller;
}

SAKTestDeviceController *SAKTestDebugPage::controllerInstance()
{
    return controller;
}

SAKDebugPageDevice *SAKTestDebugPage::createDevice()
{
    SAKTestDevice *ret = new SAKTestDevice(this);
    return ret;
}

void SAKTestDebugPage::refreshDevice()
{
    controller->refresh();
}

QWidget *SAKTestDebugPage::controllerWidget()
{
    return controller;
}

void SAKTestDebugPage::setUiEnable(bool enable)
{
    controller->setEnabled(enable);
    mRefreshPushButton->setEnabled(enable);
}
