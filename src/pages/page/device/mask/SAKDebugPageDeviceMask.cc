/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebugPageDeviceMask.hh"
#include "ui_SAKDebugPageDeviceMask.h"

class SAKDebugPage;
SAKDebugPageDeviceMask::SAKDebugPageDeviceMask(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,ui(new Ui::SAKDebugPageDeviceMask)
{
    ui->setupUi(this);
}

SAKDebugPageDeviceMask::~SAKDebugPageDeviceMask()
{
    delete ui;
}
