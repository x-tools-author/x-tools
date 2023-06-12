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

#include "SAKEmitterToolUi.hh"

SAKEmitterToolUi::SAKEmitterToolUi(QWidget *parent)
    : SAKTableModelToolUi{"SAK.EmitterToolUi", parent}
{
    mEditor = new SAKEmitterToolUiEditor(this);
}

SAKEmitterToolUi::~SAKEmitterToolUi()
{

}

void SAKEmitterToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                               const QString &settingGroup)
{
    SAKTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 11;
    setStretchSections(columns);
}

QDialog *SAKEmitterToolUi::itemEditor()
{
    return mEditor;
}
