/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsPrestorerToolUi.h"

#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QTableView>

#include "xToolsApplication.h"
#include "xToolsMenu.h"
#include "xToolsPrestorerTool.h"
#include "xToolsPrestorerToolUiEditor.h"

xToolsPrestorerToolUi::xToolsPrestorerToolUi(QWidget *parent)
    : xToolsTableModelToolUi(parent)
{
    m_editor = new xToolsPrestorerToolUiEditor(xToolsApplication::mainWindow());
    m_menu = new xToolsMenu();
}

xToolsPrestorerToolUi::~xToolsPrestorerToolUi() {}

QMenu *xToolsPrestorerToolUi::menu()
{
    return m_menu;
}

void xToolsPrestorerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                    const QString &settingGroup)
{
    xToolsTableModelToolUi::onBaseToolUiInitialized(tool, settingGroup);

    QList<int> columns;
    columns << 9;
    setStretchSections(columns);

    xToolsPrestorerTool *cookedTool = dynamic_cast<xToolsPrestorerTool *>(tool);
    auto *model = cookedTool->tableModel().value<xToolsTableModel *>();
    connect(model, &QAbstractTableModel::rowsRemoved, this, &xToolsPrestorerToolUi::updateMenu);
    connect(model, &QAbstractTableModel::rowsInserted, this, &xToolsPrestorerToolUi::updateMenu);
    connect(model, &QAbstractTableModel::dataChanged, this, &xToolsPrestorerToolUi::updateMenu);
    updateMenu();

    QList<int> list;
    list << 10;
    setStretchSections(list);
}

QList<int> xToolsPrestorerToolUi::defaultHideColumns()
{
    QList<int> list;
    auto tb = m_TableModelTool->tableModel().value<QAbstractTableModel *>();
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

void xToolsPrestorerToolUi::afterRowEdited(int row)
{
    xToolsTableModelToolUi::afterRowEdited(row);
    updateMenu();
}

QDialog *xToolsPrestorerToolUi::itemEditor()
{
    return m_editor;
}

void xToolsPrestorerToolUi::updateMenu()
{
    auto *cookedTool = dynamic_cast<xToolsPrestorerTool *>(m_TableModelTool);
    auto *model = cookedTool->tableModel().value<xToolsTableModel *>();

    m_menu->clear();

    for (int i = 0; i < model->rowCount(); i++) {
        QString desc = cookedTool->description(i);
        QAction *a = new QAction(desc, m_menu);
        m_menu->addAction(a);
        connect(a, &QAction::triggered, this, [=]() { cookedTool->send(i); });
    }
}
