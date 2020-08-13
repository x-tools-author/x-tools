/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKTestDeviceController.hh"
#include "ui_SAKTestDeviceController.h"
SAKTestDeviceController::SAKTestDeviceController(QWidget *parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKTestDeviceController)
{
    mUi->setupUi(this);
}

SAKTestDeviceController::~SAKTestDeviceController()
{
    delete mUi;
}
