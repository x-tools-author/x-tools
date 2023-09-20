/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QFile>
#include <QTimer>
#include <QMutex>
#include <QVariant>
#include <QJsonArray>
#include <QJsonDocument>
#include <QAbstractTableModel>

#include "SAKTableModelTool.h"
#include "SAKResponserToolUi.h"

SAKResponserToolUi::SAKResponserToolUi(QWidget *parent)
    : SAKTableModelToolUi{"sak.responsertoolui", parent}
{
    mEditor = new SAKResponserToolUiEditor(this);
}

SAKResponserToolUi::~SAKResponserToolUi()
{

}

void SAKResponserToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                 const QString &settingGroup)
{
    SAKTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);
    QList<int> columns;
    columns << 12 << 23;
    setStretchSections(columns);
}

QList<int> SAKResponserToolUi::defaultHideColumns()
{
    QList<int> list;
    auto tb = mTableModelTool->tableModel().value<QAbstractTableModel*>();
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
