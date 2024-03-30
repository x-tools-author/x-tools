/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsWebSocketTransmitterToolUi.h"

xToolsWebSocketTransmitterToolUi::xToolsWebSocketTransmitterToolUi(QWidget *parent)
    : xToolsSocketClientTransmitterToolUi(parent)
{}

void xToolsWebSocketTransmitterToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                            const QString &settingGroup)
{
    xToolsSocketClientTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);

    setColumnVisible(4, true);
    m_editor->setWebSocketContextVisible(true);
}
