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
#include <QTimer>
#include <QMutex>
#include <QVariant>
#include <QJsonArray>
#include <QJsonDocument>
#include <QAbstractTableModel>

#include "SAKResponserTool.hh"
#include "SAKResponserToolUi.hh"
#include "SAKResponserToolUiEditor.hh"

SAKResponserToolUi::SAKResponserToolUi(QWidget *parent)
    : SAKTableModelToolUi{"SAK.ResponserToolUi", parent}
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
    columns << 11 << 21;
    setStretchSections(columns);
}

QDialog *SAKResponserToolUi::itemEditor()
{
    return mEditor;
}
