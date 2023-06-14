/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSocketClientTransmitterToolUi.hh"

SAKSocketClientTransmitterToolUi::SAKSocketClientTransmitterToolUi(
    const char *lg, QWidget *parent)
    : SAKTransmitterToolUi(lg, parent)
{
    mEditor = new SAKSocketClientTransmitterToolUiEditor(this);
}

QDialog *SAKSocketClientTransmitterToolUi::itemEditor()
{
    return mEditor;
}

void SAKSocketClientTransmitterToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingGroup)
{
    SAKTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 5;
    setStretchSections(columns);
    setColumnVisible(6, false);
}
