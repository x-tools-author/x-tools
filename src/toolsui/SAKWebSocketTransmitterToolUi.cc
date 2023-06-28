/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKWebSocketTransmitterToolUi.hh"

SAKWebSocketTransmitterToolUi::SAKWebSocketTransmitterToolUi(QWidget *parent)
    : SAKSocketClientTransmitterToolUi("SAK.WebSocketTransmitterToolUi", parent)
{

}

void SAKWebSocketTransmitterToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingGroup)
{
    SAKSocketClientTransmitterToolUi::onBaseToolUiInitialized(tool,
                                                              settingGroup);

    setColumnVisible(4, true);
    mEditor->setWebSocketContextVisible(true);
}
