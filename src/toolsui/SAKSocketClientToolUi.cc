/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSocketClientTool.hh"
#include "SAKCommunicationTool.hh"
#include "SAKSocketClientToolUi.hh"
#include "ui_SAKSocketClientToolUi.h"

SAKSocketClientToolUi::SAKSocketClientToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSocketClientToolUi)
{
    ui->setupUi(this);
}

SAKSocketClientToolUi::~SAKSocketClientToolUi()
{
    delete ui;
}

void SAKSocketClientToolUi::setupCommunicationTool(SAKCommunicationTool *tool)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKSocketClientTool")) {
        qWarning(mLoggingCategory) << "Invalid SAKSocketClientTool object,"
                                      " the operation will be ignored!";
        return;
    }

    if (!tool->inherits("SAKWebSocketClientTool")) {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }
}

void SAKSocketClientToolUi::updateUiState(bool isWorking)
{
    Q_UNUSED(isWorking)
}
