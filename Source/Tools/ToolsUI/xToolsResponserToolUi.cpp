/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsResponserToolUi.h"

#include <QAbstractTableModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMutex>
#include <QTimer>
#include <QVariant>

#include "xToolsTableModelTool.h"
#include "xToolsUiInterface.h"

xToolsResponserToolUi::xToolsResponserToolUi(QWidget *parent)
    : xToolsTableModelToolUi{"sak.responsertoolui", parent}
{
    mEditor = new xToolsResponserToolUiEditor(xToolsUiInterface::mainWindow());
}

xToolsResponserToolUi::~xToolsResponserToolUi() {}

void xToolsResponserToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingGroup)
{
    xToolsTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);
    QList<int> columns;
    columns << 12 << 23;
    setStretchSections(columns);
}

QList<int> xToolsResponserToolUi::defaultHideColumns()
{
    QList<int> list;
    auto tb = mTableModelTool->tableModel().value<QAbstractTableModel *>();
    for (int i = 0; i < tb->columnCount(); i++) {
        list.append(i);
    }

    list.removeAll(0);
    list.removeAll(1);
    list.removeAll(2);
    list.removeAll(12);
    list.removeAll(23);

    return list;
}

QDialog *xToolsResponserToolUi::itemEditor()
{
    return mEditor;
}
