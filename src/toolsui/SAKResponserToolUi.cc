/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
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

#include "SAKResponserToolUi.hh"

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
    columns << 12 << 24;
    setStretchSections(columns);
}

QDialog *SAKResponserToolUi::itemEditor()
{
    return mEditor;
}
