/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakserialporttransmittertoolui.h"

#include "sakuiinterface.h"

SAKSerialPortTransmitterToolUi::SAKSerialPortTransmitterToolUi(QWidget *parent)
    : SAKTransmitterToolUi("SAK.SerialPortTransmitterToolUi", parent)
{
    mEditor = new SAKSerialPortTransmitterToolUiEditor(SAKUiInterface::mainWindow());
}

void SAKSerialPortTransmitterToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                             const QString &settingGroup)
{
    SAKTransmitterToolUi::SAKTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);
    setSectionResizeModeToStretch();
}

QDialog *SAKSerialPortTransmitterToolUi::itemEditor()
{
    return mEditor;
}
