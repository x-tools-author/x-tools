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

void SAKAnalyzerToolUi::setupAnalyzer(SAKAnalyzerTool *tool)
{
    Q_UNUSED(tool)
}

void SAKAnalyzerToolUi::setupSettingsGroup(const QString &group)
{
    ui->checkBoxEnable->setGroupKey(group, "analyzerEnable");
    ui->checkBoxFixedLength->setGroupKey(group, "fxiedLength");
    ui->spinBoxFrameLength->setGroupKey(group, "frameLength");
    ui->spinBoxMaxTempBytes->setGroupKey(group, "maxTempBytes");
    ui->lineEditHeader->setGroupKey(group, "header");
    ui->lineEditTail->setGroupKey(group, "tail");
}
