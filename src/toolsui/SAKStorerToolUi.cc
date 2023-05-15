/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKStorerToolUi.hh"
#include "ui_SAKStorerToolUi.h"

SAKStorerToolUi::SAKStorerToolUi(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKStorerToolUi)
{
    ui->setupUi(this);
}

SAKStorerToolUi::~SAKStorerToolUi()
{
    delete ui;
}

void SAKStorerToolUi::setupStorer(SAKStorerTool *tool)
{
    Q_UNUSED(tool)
}
