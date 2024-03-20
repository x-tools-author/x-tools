/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSerialPortTransmitterToolUi.h"

#include "sakuiinterface.h"

xToolsSerialPortTransmitterToolUi::xToolsSerialPortTransmitterToolUi(QWidget *parent)
    : xToolsTransmitterToolUi("SAK.SerialPortTransmitterToolUi", parent)
{
    mEditor = new xToolsSerialPortTransmitterToolUiEditor(SAKUiInterface::mainWindow());
}

void xToolsSerialPortTransmitterToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                             const QString &settingGroup)
{
    xToolsTransmitterToolUi::xToolsTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);
    setSectionResizeModeToStretch();
}

QDialog *xToolsSerialPortTransmitterToolUi::itemEditor()
{
    return mEditor;
}
