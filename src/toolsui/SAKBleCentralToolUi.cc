/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKBleCentralToolUi.hh"
#include "SAKCommunicationTool.hh"
#include "ui_SAKBleCentralToolUi.h"

SAKBleCentralToolUi::SAKBleCentralToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKBleCentralToolUi)
{
    ui->setupUi(this);
}

SAKBleCentralToolUi::~SAKBleCentralToolUi()
{
    delete ui;
}

void SAKBleCentralToolUi::setupCommunicationTool(SAKCommunicationTool *tool)
{
    Q_UNUSED(tool)
}

void SAKBleCentralToolUi::updateUiState(bool isWorking)
{
    Q_UNUSED(isWorking)
}
