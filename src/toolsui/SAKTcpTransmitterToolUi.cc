/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKTcpTransmitterToolUi.hh"

SAKTcpTransmitterToolUi::SAKTcpTransmitterToolUi(QWidget *parent)
    : SAKTransmitterToolUi{"SAK.TcpTransmitterToolUi", parent}
{
    mEditor = new SAKTcpTransmitterToolUiEditor(this);
}

QDialog *SAKTcpTransmitterToolUi::itemEditor()
{
    return mEditor;
}


void SAKTcpTransmitterToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingGroup)
{
    SAKTransmitterToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 5;
    setStretchSections(columns);
}
