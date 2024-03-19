/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QAbstractTableModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMutex>
#include <QTimer>
#include <QVariant>

#include "sakresponsertoolui.h"
#include "saktablemodeltool.h"
#include "sakuiinterface.h"

SAKResponserToolUi::SAKResponserToolUi(QWidget *parent)
    : SAKTableModelToolUi{"sak.responsertoolui", parent}
{
    mEditor = new SAKResponserToolUiEditor(SAKUiInterface::mainWindow());
}

SAKResponserToolUi::~SAKResponserToolUi() {}

void SAKResponserToolUi::onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingGroup)
{
    SAKTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);
    QList<int> columns;
    columns << 12 << 23;
    setStretchSections(columns);
}

QList<int> SAKResponserToolUi::defaultHideColumns()
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

QDialog *SAKResponserToolUi::itemEditor()
{
    return mEditor;
}
