/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKMaskerToolUi.hh"
#include "ui_SAKMaskerToolUi.h"

SAKMaskerToolUi::SAKMaskerToolUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SAKMaskerToolUi)
{
    ui->setupUi(this);
}

SAKMaskerToolUi::~SAKMaskerToolUi()
{
    delete ui;
}

void SAKMaskerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}
