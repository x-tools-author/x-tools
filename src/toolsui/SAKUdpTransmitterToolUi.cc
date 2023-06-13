/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKUdpTransmitterToolUi.hh"

SAKUdpTransmitterToolUi::SAKUdpTransmitterToolUi(QWidget *parent)
    : SAKTransmitterToolUi{"SAK.UdpTransmitterToolUi", parent}
{
    mEditor = new SAKUdpTransmitterToolUiEditor(this);
}

QDialog *SAKUdpTransmitterToolUi::itemEditor()
{
    return mEditor;
}

void SAKUdpTransmitterToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingGroup)
{
    SAKTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 5;
    setStretchSections(columns);
}
