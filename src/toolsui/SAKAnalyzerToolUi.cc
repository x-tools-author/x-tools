/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKAnalyzerToolUi.hh"
#include "ui_SAKAnalyzerToolUi.h"

SAKAnalyzerToolUi::SAKAnalyzerToolUi(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKAnalyzerToolUi)
{
    ui->setupUi(this);
}

void SAKAnalyzerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}
