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

#include "SAKCommonDataStructure.hh"
#include "SAKTestDevice.hh"
#include "SAKTestDebugPage.hh"
#include "SAKTestDeviceController.hh"

SAKTestDebugPage::SAKTestDebugPage(int type, QString name, QWidget *parent)
    :SAKDebugPage(type, name, parent)
{
    mDeviceController = new SAKTestDeviceController(this);
    mDevice = new SAKTestDevice(this, this);
    initializePage();
}
