/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QTableView>

#include "sakmenu.h"
#include "sakprestorertool.h"
#include "sakprestorertoolui.h"
#include "sakprestorertooluieditor.h"
#include "sakuiinterface.h"

SAKPrestorerToolUi::SAKPrestorerToolUi(QWidget *parent)
    : SAKTableModelToolUi{"SAK.PrestorerToolUi", parent}
{
    mEditor = new SAKPrestorerToolUiEditor(SAKUiInterface::mainWindow());
    mMenu = new SAKMenu();
}

SAKPrestorerToolUi::~SAKPrestorerToolUi() {}

QMenu *SAKPrestorerToolUi::menu()
{
    return mMenu;
}

void SAKPrestorerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingGroup)
{
    SAKTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 9;
    setStretchSections(columns);

    SAKPrestorerTool *cookedTool = qobject_cast<SAKPrestorerTool *>(tool);
    auto *model = cookedTool->tableModel().value<SAKTableModel *>();
    connect(model, &QAbstractTableModel::rowsRemoved, this, &SAKPrestorerToolUi::updateMenu);
    connect(model, &QAbstractTableModel::rowsInserted, this, &SAKPrestorerToolUi::updateMenu);
    connect(model, &QAbstractTableModel::dataChanged, this, &SAKPrestorerToolUi::updateMenu);
    updateMenu();

    QList<int> list;
    list << 10;
    setStretchSections(list);
}

QList<int> SAKPrestorerToolUi::defaultHideColumns()
{
    QList<int> list;
    auto tb = mTableModelTool->tableModel().value<QAbstractTableModel *>();
    for (int i = 0; i < tb->columnCount(); i++) {
        list.append(i);
    }

    list.removeAll(0);
    list.removeAll(1);
    list.removeAll(2);
    list.removeAll(3);
    list.removeAll(4);
    list.removeAll(10);

    return list;
}

void SAKPrestorerToolUi::afterRowEdited(int row)
{
    SAKTableModelToolUi::afterRowEdited(row);
    updateMenu();
}

QDialog *SAKPrestorerToolUi::itemEditor()
{
    return mEditor;
}

void SAKPrestorerToolUi::updateMenu()
{
    auto *cookedTool = qobject_cast<SAKPrestorerTool *>(mTableModelTool);
    auto *model = cookedTool->tableModel().value<SAKTableModel *>();

    mMenu->clear();

    for (int i = 0; i < model->rowCount(); i++) {
        QString desc = cookedTool->description(i);
        QAction *a = new QAction(desc, mMenu);
        mMenu->addAction(a);
        connect(a, &QAction::triggered, this, [=]() { cookedTool->send(i); });
    }
}
