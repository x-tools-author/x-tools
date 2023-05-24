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
    : SAKBaseToolUi{parent}
    , ui(new Ui::SAKAnalyzerToolUi)
{
    ui->setupUi(this);
}

void SAKAnalyzerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}

void SAKAnalyzerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                const QString &settingsGroup)
{
    ui->checkBoxEnable->setGroupKey(settingsGroup, "analyzerEnable");
    ui->checkBoxFixedLength->setGroupKey(settingsGroup, "fxiedLength");
    ui->spinBoxFrameLength->setGroupKey(settingsGroup, "frameLength");
    ui->spinBoxMaxTempBytes->setGroupKey(settingsGroup, "maxTempBytes");
    ui->lineEditHeader->setGroupKey(settingsGroup, "header");
    ui->lineEditTail->setGroupKey(settingsGroup, "tail");

    Q_UNUSED(tool)
}
