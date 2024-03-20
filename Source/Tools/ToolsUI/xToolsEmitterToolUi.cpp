/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakemittertoolui.h"

#include "xToolsTableModelTool.h"
#include "sakuiinterface.h"

SAKEmitterToolUi::SAKEmitterToolUi(QWidget *parent)
    : SAKTableModelToolUi{"sak.emittertoolui", parent}
{
    mEditor = new SAKEmitterToolUiEditor(SAKUiInterface::mainWindow());
}

SAKEmitterToolUi::~SAKEmitterToolUi() {}

void SAKEmitterToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingGroup)
{
    SAKTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 12;
    setStretchSections(columns);
}

QList<int> SAKEmitterToolUi::defaultHideColumns()
{
    QList<int> list;
    auto tb = mTableModelTool->tableModel().value<QAbstractTableModel *>();
    for (int i = 0; i < tb->columnCount(); i++) {
        list.append(i);
    }

    list.removeAll(0);
    list.removeAll(1);
    list.removeAll(4);
    list.removeAll(12);

    return list;
}

QDialog *SAKEmitterToolUi::itemEditor()
{
    return mEditor;
}
