/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKBaseTool.hh"
#include "SAKBaseToolUi.hh"

SAKBaseToolUi::SAKBaseToolUi(QWidget *parent)
    : QWidget{parent}
{

}

void SAKBaseToolUi::onIsWorkingChanged(bool isWorking)
{
    Q_UNUSED(isWorking)
}

void SAKBaseToolUi::setupTool(SAKBaseTool *tool)
{
    mTool = tool;
}

void SAKBaseToolUi::setupSettingsGroup(const QString &group)
{
    mSettingsGroup = group;
}
