/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakserialporttransmittertoolui.h"

SAKSerialPortTransmitterToolUi::SAKSerialPortTransmitterToolUi(QWidget *parent)
    : SAKTransmitterToolUi("SAK.SerialPortTransmitterToolUi", parent)
{
    mEditor = new SAKSerialPortTransmitterToolUiEditor(this);
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
