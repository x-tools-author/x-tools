/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsEmitterToolUi.h"

#include "xToolsApplication.h"
#include "xToolsTableModelTool.h"

xToolsEmitterToolUi::xToolsEmitterToolUi(QWidget *parent)
    : xToolsTableModelToolUi{parent}
{
    m_editor = new xToolsEmitterToolUiEditor(xToolsApplication::mainWindow());
}

xToolsEmitterToolUi::~xToolsEmitterToolUi() {}

void xToolsEmitterToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingGroup)
{
    xToolsTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 12;
    setStretchSections(columns);
}

QList<int> xToolsEmitterToolUi::defaultHideColumns()
{
    QList<int> list;
    auto tb = m_TableModelTool->tableModel().value<QAbstractTableModel *>();
    for (int i = 0; i < tb->columnCount(); i++) {
        list.append(i);
    }

    list.removeAll(0);
    list.removeAll(1);
    list.removeAll(4);
    list.removeAll(12);

    return list;
}

QDialog *xToolsEmitterToolUi::itemEditor()
{
    return m_editor;
}
