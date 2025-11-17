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
#include <QPushButton>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>

#include "common/xtools.h"
#include "presetmodel.h"
#include "presetviewgroupeditor.h"
#include "utilities/keepopenedmenu.h"

PresetView::PresetView(QWidget *parent)
    : TableView(parent)
{
    m_menu = new xTools::KeepOpenedMenu();

    setIdDisableCheckBoxVisible(false);

    m_tableModel = new PresetModel(this);
    connect(m_tableModel, &QAbstractTableModel::dataChanged, this, &PresetView::onDataChanged);
    connect(m_tableModel, &QAbstractTableModel::rowsRemoved, this, &PresetView::onDataChanged);
    connect(m_tableModel, &QAbstractTableModel::rowsInserted, this, &PresetView::onDataChanged);
    setTableModel(m_tableModel);

    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    hHeader->setStretchLastSection(true);

    const QString title = tr("Group Editor");
    m_groupEditor = new PresetViewGroupEditor();
    m_groupEditor->hide();
    m_groupEditor->setWindowTitle(title);
    connect(m_groupEditor, &PresetViewGroupEditor::invokeSend, this, &PresetView::onInvokeSend);

    QPushButton *btn = new QPushButton(this);
    btn->setText(title);
    addControlWidgets(btn);
    setVerticalHeaderVisible(true);
    setMoveRowEnabled(true);

    connect(btn, &QPushButton::clicked, this, [this]() {
        m_groupEditor->show();
        m_groupEditor->raise();
        m_groupEditor->activateWindow();
    });
}

PresetView::~PresetView()
{
    m_menu->deleteLater();
    m_groupEditor->deleteLater();
}

QMenu *PresetView::menu()
{
    return m_menu;
}

QVariantMap PresetView::save() const
{
    QVariantMap map = TableView::save();
    map["groups"] = m_groupEditor->save().toVariantMap();
    return map;
}

void PresetView::load(const QVariantMap &parameters)
{
    TableView::load(parameters);
    if (parameters.contains("groups")) {
        QVariantMap map = parameters.value("groups").toMap();
        QJsonObject obj = QJsonObject::fromVariantMap(map);
        m_groupEditor->load(obj);
    }
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
    m_menu->addAction(tr("Edit Data"), this, [this]() {
        emit invokeComeHere();
        m_menu->hide();
    });

    m_menu->addMenu(m_groupEditor->groupMenu());
    m_menu->addSeparator();
    int rows = m_tableModel->rowCount(QModelIndex());

    for (int i = 0; i < rows; ++i) {
        auto index = m_tableModel->index(i, 0);
        auto text = m_tableModel->data(index, Qt::DisplayRole).toString();
        m_menu->addAction(text, this, [=]() { onActionTriggered(i); });
    }
}

void PresetView::onInvokeSend(int row)
{
    if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
        return;
    }

    onActionTriggered(row);
}