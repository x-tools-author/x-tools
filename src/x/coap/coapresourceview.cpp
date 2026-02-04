/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapresourceview.h"
#include "ui_coapresourceview.h"

#include <QJsonArray>

#include "utilities/compatibility.h"
#include "utilities/iconengine.h"

#include "coapcommon.h"
#include "coapresourceeditor.h"
#include "coapresourcefilter.h"
#include "coapresourcemodel.h"

namespace xCoAP {

class CoAPResourceViewPrivate : public QObject
{
public:
    CoAPResourceViewPrivate(CoAPResourceView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPResourceView();
        ui->setupUi(q);
        ui->toolButtonAdd->setIcon(xIcon(":res/icons/add.svg"));
        ui->toolButtonDelete->setIcon(xIcon(":res/icons/delete.svg"));
        ui->toolButtonClear->setIcon(xIcon(":res/icons/mop.svg"));
        connect(ui->toolButtonAdd, &QToolButton::clicked, q, [=]() { onAddBtnClicked(); });
        connect(ui->toolButtonDelete, &QToolButton::clicked, q, [=]() { onDeleteBtnClicked(); });
        connect(ui->toolButtonClear, &QToolButton::clicked, q, [=]() { onClearBtnClicked(); });
        connect(ui->comboBoxFilter, xComboBoxActivated, q, [=]() {
            qCDebug(xCoAPLog) << "Filter format changed:" << ui->comboBoxFilter->currentText();
            m_filter->setFormat(ui->comboBoxFilter->currentData().toInt());
        });
        connect(ui->lineEditFilter, &QLineEdit::textChanged, q, [=](const QString& text) {
            qCDebug(xCoAPLog) << "Filter text changed:" << text;
            m_filter->setFilterText(text);
        });

        m_model = new CoAPResourceModel(q);
        m_filter = new CoAPResourceFilter(q);
        m_filter->setSourceModel(m_model);
        ui->tableView->setModel(m_filter);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->setAlternatingRowColors(true);
        connect(ui->tableView, &QTableView::doubleClicked, q, [=](const QModelIndex& index) {
            onDoubleClicked(m_filter->mapToSource(index).row());
        });

        CoAPCommon::setupContextFormat(ui->comboBoxFilter);
        ui->comboBoxFilter->insertItem(0, tr("All"), CO_AP_INVALID_CONTEXT_FORMAT);

        m_editor = new CoAPResourceEditor();
        connect(m_editor, &CoAPResourceEditor::accepted, this, [=]() { onAccepted(); });
    }
    ~CoAPResourceViewPrivate() override
    {
        m_editor->deleteLater();
        delete ui;
    }

public:
    Ui::CoAPResourceView* ui{nullptr};
    CoAPResourceModel* m_model{nullptr};
    CoAPResourceFilter* m_filter{nullptr};

private:
    CoAPResourceView* q{nullptr};
    CoAPResourceEditor* m_editor{nullptr};

private:
    void onAddBtnClicked()
    {
        m_editor->show();
        CoAPCommon::ResourceItem item = CoAPCommon::jsonObject2ResourceItem(QJsonObject());
        m_editor->setEditingRow(CO_AP_INVALID_CONTEXT_FORMAT);
        m_editor->load(CoAPCommon::resourceItem2JsonObject(item));
    }
    void onDeleteBtnClicked()
    {
        if (!CoAPCommon::deleteSelectedItems()) {
            return;
        }

        QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            qCDebug(xCoAPLog) << "No selected indexes to remove.";
            return;
        }

        QList<int> rowList;
        for (const QModelIndex& index : std::as_const(selectedIndexes)) {
            int row = m_filter->mapToSource(index).row();
            if (!rowList.contains(row)) {
                rowList.append(row);
            }
        }

        std::sort(rowList.begin(), rowList.end(), std::greater<int>());
        for (int i = 0; i < rowList.count(); i++) {
            m_model->deleteRow(rowList.first());
        }
    }
    void onClearBtnClicked()
    {
        if (!CoAPCommon::clearAllItems()) {
            return;
        }

        m_model->clearAllRows();
    }
    void onAccepted()
    {
        const int row = m_editor->editingRow();
        const QJsonObject obj = m_editor->save();
        if (row == CO_AP_INVALID_CONTEXT_FORMAT) {
            m_model->addResource(obj);
        } else {
            m_model->updateResource(row, obj);
        }
    }
    void onDoubleClicked(int row)
    {
        QJsonObject obj = m_model->saveRow(row);
        if (obj.isEmpty()) {
            return;
        }

        m_editor->setEditingRow(row);
        m_editor->load(obj);
        m_editor->show();
    }
};

struct CoAPResourceViewParameterKeys
{
    const QString resourceItems{"resourceItems"};
    const QString filterFormat{"filterFormat"};
    const QString filterText{"filterText"};
};

CoAPResourceView::CoAPResourceView(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPResourceViewPrivate(this);
}

CoAPResourceView::~CoAPResourceView() {}

QJsonObject CoAPResourceView::save()
{
    QJsonObject obj;
    CoAPResourceViewParameterKeys keys;
    QJsonArray resArray;
    for (int row = 0; row < d->m_model->rowCount(QModelIndex()); ++row) {
        resArray.append(d->m_model->saveRow(row));
    }
    obj[keys.resourceItems] = resArray;
    obj[keys.filterFormat] = d->ui->comboBoxFilter->currentData().toInt();
    obj[keys.filterText] = d->ui->lineEditFilter->text().trimmed();
    return obj;
}

void CoAPResourceView::load(const QJsonObject& obj)
{
    CoAPResourceViewParameterKeys keys;
    QJsonArray resArray = obj.value(keys.resourceItems).toArray();
    for (const QJsonValue& val : std::as_const(resArray)) {
        d->m_model->addResource(val.toObject());
    }

    int index = d->ui->comboBoxFilter->findData(obj.value(keys.filterFormat).toInt());
    d->ui->comboBoxFilter->setCurrentIndex(index >= 0 ? index : 0);
    d->ui->lineEditFilter->setText(obj.value(keys.filterText).toString(""));
    d->m_filter->setFilterText(d->ui->lineEditFilter->text().trimmed());
    d->m_filter->setFormat(d->ui->comboBoxFilter->currentData().toInt());
}

CoAPResourceModel* CoAPResourceView::resourceModel() const
{
    return d->m_model;
}

} // namespace xCoAP