/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKCommunicationTool.hh"
#include "SAKSocketServerToolUi.hh"
#include "ui_SAKSocketServerToolUi.h"

SAKSocketServerToolUi::SAKSocketServerToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSocketServerToolUi)
{
    ui->setupUi(this);
}

SAKSocketServerToolUi::~SAKSocketServerToolUi()
{
    delete ui;
}

void SAKSocketServerToolUi::setupCommunicationTool(SAKCommunicationTool *tool)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKWebSocketServerTool")) {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }
}

void SAKSocketServerToolUi::updateUiState(bool isWorking)
{
    Q_UNUSED(isWorking)
}
