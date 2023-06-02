/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QJsonArray>
#include <QTableView>
#include <QHeaderView>
#include <QJsonDocument>
#include <QStandardItemModel>

#include "SAKPrestorerTool.hh"
#include "SAKPrestorerToolUi.hh"
#include "SAKPrestorerToolUiEditor.hh"

SAKPrestorerToolUi::SAKPrestorerToolUi(QWidget *parent)
    : SAKTableModelToolUi{"SAK.PrestorerToolUi", parent}
{
    mEditor = new SAKPrestorerToolUiEditor(this);
}

SAKPrestorerToolUi::~SAKPrestorerToolUi()
{

}

void SAKPrestorerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                 const QString &settingGroup)
{
    SAKTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 9;
    setStretchSections(columns);
}

QDialog *SAKPrestorerToolUi::itemEditor()
{
    return mEditor;
}
