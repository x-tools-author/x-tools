/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saksocketclienttransmittertoolui.h"

#include "sakuiinterface.h"

SAKSocketClientTransmitterToolUi::SAKSocketClientTransmitterToolUi(const char *lg, QWidget *parent)
    : SAKTransmitterToolUi(lg, parent)
{
    mEditor = new SAKSocketClientTransmitterToolUiEditor(SAKUiInterface::mainWindow());
}

QDialog *SAKSocketClientTransmitterToolUi::itemEditor()
{
    return mEditor;
}

void SAKSocketClientTransmitterToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                               const QString &settingGroup)
{
    SAKTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);
    setColumnVisible(4, false);
    setSectionResizeModeToStretch();
    mEditor->setWebSocketContextVisible(false);
}
