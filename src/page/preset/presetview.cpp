/***************************************************************************************************
 * Copyright 2023-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QToolButton>

#include "common/x.h"
#include "presetmodel.h"
#include "presetviewgroupeditor.h"
#include "utilities/keepopenedmenu.h"

class PresetViewPrivate : public QObject
{
public:
    PresetViewPrivate(PresetView *q_ptr)
        : q(q_ptr)
    {}
    ~PresetViewPrivate() {}

public:
    void onActionTriggered(int row)
    {
        auto rows = m_tableModel->rowCount(QModelIndex());
        if (row < 0 || row >= rows) {
            return;
        }

        QModelIndex index = m_tableModel->index(row, PRESET_MODEL_COLUMN_DATA);
        QJsonObject rawItem = m_tableModel->data(index, Qt::EditRole).toJsonObject();
        TextItem textItem = xLoadTextItem(rawItem);
        QByteArray bytes = xTextItem2array(textItem);
        emit q->outputBytes(bytes);
    }
    void onInvokeSend(int row)
    {
        if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
            return;
        }

        onActionTriggered(row);
    }

public:
    PresetModel *m_tableModel;
    PresetViewGroupEditor *m_groupEditor{nullptr};

private:
    PresetView *q;
};

PresetView::PresetView(QWidget *parent)
    : TableView(parent)
{
    d = new PresetViewPrivate(this);

    setIdDisableCheckBoxVisible(false);

    d->m_tableModel = new PresetModel(this);
    setTableModel(d->m_tableModel);

    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    hHeader->setStretchLastSection(true);

    const QString title = tr("Group Editor");
    d->m_groupEditor = new PresetViewGroupEditor();
    d->m_groupEditor->hide();
    d->m_groupEditor->setWindowTitle(title);
    connect(d->m_groupEditor,
            &PresetViewGroupEditor::invokeSend,
            d,
            &PresetViewPrivate::onInvokeSend);

    QToolButton *btn = new QToolButton(this);
    btn->setText(title);
    addControlWidgets(btn);
    setVerticalHeaderVisible(true);
    setMoveRowEnabled(true);
    btn->setMenu(d->m_groupEditor->groupMenu());
    btn->setPopupMode(QToolButton::MenuButtonPopup);

    connect(btn, &QToolButton::clicked, this, [this]() {
        d->m_groupEditor->show();
        d->m_groupEditor->raise();
        d->m_groupEditor->activateWindow();
    });
}

PresetView::~PresetView()
{
    d->m_groupEditor->deleteLater();
}

QVariantMap PresetView::save() const
{
    QVariantMap map = TableView::save();
    map["groups"] = d->m_groupEditor->save().toVariantMap();
    return map;
}

void PresetView::load(const QVariantMap &parameters)
{
    TableView::load(parameters);
    if (parameters.contains("groups")) {
        QVariantMap map = parameters.value("groups").toMap();
        QJsonObject obj = QJsonObject::fromVariantMap(map);
        d->m_groupEditor->load(obj);
    }
}

QList<int> PresetView::textItemColumns() const
{
    return QList<int>{PRESET_MODEL_COLUMN_DATA};
}

void PresetView::onCellDoubleClicked(const QModelIndex &index)
{
    int column = index.column();
    if (column == PRESET_MODEL_COLUMN_SEND) {
        d->onActionTriggered(index.row());
    } else {
        TableView::onCellDoubleClicked(index);
    }
}