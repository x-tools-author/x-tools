/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketClientTransmitterToolUi.h"

#include "xToolsApplication.h"

xToolsSocketClientTransmitterToolUi::xToolsSocketClientTransmitterToolUi(QWidget *parent)
    : xToolsTransmitterToolUi(parent)
{
    m_editor = new xToolsSocketClientTransmitterToolUiEditor(xToolsApplication::mainWindow());
}

QDialog *xToolsSocketClientTransmitterToolUi::itemEditor()
{
    return m_editor;
}

void xToolsSocketClientTransmitterToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                               const QString &settingGroup)
{
    xToolsTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);
    setColumnVisible(4, false);
    setSectionResizeModeToStretch();
    m_editor->setWebSocketContextVisible(false);
}
