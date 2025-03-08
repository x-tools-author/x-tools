/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "presetview.h"

#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>

#include "common/xtools.h"
#include "page/utilities/menu.h"
#include "presetmodel.h"

PresetView::PresetView(QWidget *parent)
    : TableView(parent)
{
    m_menu = new Menu();

    setIdDisableCheckBoxVisible(false);

    m_tableModel = new PresetModel(this);
    connect(m_tableModel, &QAbstractTableModel::dataChanged, this, &PresetView::onDataChanged);
    connect(m_tableModel, &QAbstractTableModel::rowsRemoved, this, &PresetView::onDataChanged);
    connect(m_tableModel, &QAbstractTableModel::rowsInserted, this, &PresetView::onDataChanged);
    setTableModel(m_tableModel);

    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

PresetView::~PresetView() {}

QMenu *PresetView::menu()
{
    return m_menu;
}

QList<int> PresetView::textItemColumns() const
{
    return QList<int>{1};
}

void PresetView::onActionTriggered(int row)
{
    auto rows = m_tableModel->rowCount(QModelIndex());
    if (row < 0 || row >= rows) {
        return;
    }

    QModelIndex index = m_tableModel->index(row, 1);
    QJsonObject rawItem = m_tableModel->data(index, Qt::EditRole).toJsonObject();
    TextItem textItem = loadTextItem(rawItem);
    QByteArray bytes = textItem2array(textItem);
    emit outputBytes(bytes);
}

void PresetView::onDataChanged()
{
    m_menu->clear();
    int rows = m_tableModel->rowCount(QModelIndex());

    for (int i = 0; i < rows; ++i) {
        auto index = m_tableModel->index(i, 0);
        auto text = m_tableModel->data(index, Qt::DisplayRole).toString();
        m_menu->addAction(text, this, [=]() { onActionTriggered(i); });
    }
}
