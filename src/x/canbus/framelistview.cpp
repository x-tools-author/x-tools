/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistview.h"
#include "ui_framelistview.h"

#include <QDialog>
#include <QJsonArray>
#include <QMessageBox>

#include "common/xtools.h"
#include "utilities/iconengine.h"

#include "framelistfilter.h"
#include "framelistitemeditor.h"
#include "framelistmodel.h"

namespace Ui {
class FrameListView;
} // namespace Ui

namespace xCanBus {

struct FrameListViewParameterKeys
{
    const QString importFilePath{"importFilePath"};
    const QString exportFilePath{"exportFilePath"};
    const QString frameIdFilter{"frameIdFilter"};
    const QString payloadFilter{"payloadFilter"};

    const QString frameItems{"frameItems"};
};

class FrameListViewPrivate : public QObject
{
public:
    FrameListViewPrivate(FrameListView *parent)
        : QObject(parent)
        , q(parent)
    {
        QDialog *dialog = new QDialog(q);
        m_frameEditor = new FrameListItemEditor(dialog);
        dialog->setLayout(new QVBoxLayout);
        dialog->layout()->addWidget(m_frameEditor);
        dialog->layout()->setContentsMargins(0, 0, 0, 0);
        dialog->setWindowTitle(m_frameEditor->windowTitle());
        dialog->adjustSize();
        QObject::connect(m_frameEditor, &FrameListItemEditor::accepted, dialog, &QDialog::accept);
        QObject::connect(m_frameEditor, &FrameListItemEditor::rejected, dialog, &QDialog::reject);
        m_frameEditorDialog = dialog;
    }

public:
    void onClearBtnClicked()
    {
        if (QMessageBox::question(q,
                                  tr("Clear Frames"),
                                  tr("Are you sure you want to clear all frames?"))
            == QMessageBox::Yes) {
            m_model->clear();
        }
    }
    void onAddBtnClicked()
    {
        if (m_frameEditorDialog->exec() != QDialog::Accepted) {
            return;
        }

        FrameItem frameItem = m_frameEditor->frameItem();
        if (m_model->hasFrameId(frameItem.frame.frameId())) {
            QCanBusFrame::FrameId frameId = frameItem.frame.frameId();
            QString message = tr("A frame with the same ID (%1) already exists.").arg(frameId);
            QString question = tr("Do you want to replace the existing frame with the new one?");
            int ret = QMessageBox::question(q, tr("Duplicate Frame ID"), message + "\n" + question);
            if (ret != QMessageBox::Yes) {
                return;
            }
        }

        m_model->addFrameItem(frameItem);
    }
    void onDeleteBtnClicked()
    {
        QModelIndex index = ui->tableView->currentIndex();
        if (!index.isValid()) {
            QMessageBox::warning(q, tr("Delete Frame"), tr("Please select a frame to delete."));
            return;
        }

        int row = m_filter->mapToSource(index).row();
        if (row < 0 || row >= m_model->rowCount(QModelIndex())) {
            QMessageBox::warning(q, tr("Delete Frame"), tr("Invalid frame selected."));
            return;
        }

        const QString title = tr("Delete Frame");
        const QString message = tr("Are you sure you want to delete the selected frame?");
        if (QMessageBox::question(q, title, message) == QMessageBox::Yes) {
            m_model->removeFrameItemRow(row);
        }
    }
    void onExportBtnClicked() { }
    void onImportBtnClicked() { }
    void onFilterText(const QString &text) { Q_UNUSED(text); }
    void onViewDoubleClicked(const QModelIndex &index)
    {
        QList<FrameItem> items = m_model->frameItems();
        if (index.row() < 0 || index.row() >= items.count()) {
            return;
        }

        // If the "Send" column is double-clicked, emit the frame for sending instead of opening the editor.
        if (index.column() == FRAME_LIST_MODEL_COLUMN_SEND) {
            FrameItem item = items.at(index.row());
            emit q->outputFrame(item.frame);
            return;
        }

        // Open the editor dialog for other columns.
        FrameItem item = items.at(index.row());
        m_frameEditor->setFrameItem(item);
        if (m_frameEditorDialog->exec() != QDialog::Accepted) {
            return;
        }

        FrameItem updatedItem = m_frameEditor->frameItem();
        m_model->addFrameItem(updatedItem);
    }

public:
    Ui::FrameListView *ui{nullptr};
    FrameListModel *m_model{nullptr};
    FrameListFilter *m_filter{nullptr};

private:
    FrameListView *q;
    QDialog *m_frameEditorDialog{nullptr};
    FrameListItemEditor *m_frameEditor{nullptr};
};

FrameListView::FrameListView(QWidget *parent)
    : CanBusPanel(parent)
{
    d = new FrameListViewPrivate(this);
    d->ui = new Ui::FrameListView;
    d->ui->setupUi(this);
    d->ui->tableView->setAlternatingRowColors(true);

    // clang-format off
    connect(d->ui->toolButtonClear, &QToolButton::clicked, d, &FrameListViewPrivate::onClearBtnClicked);
    connect(d->ui->toolButtonAdd, &QToolButton::clicked, d, &FrameListViewPrivate::onAddBtnClicked);
    connect(d->ui->toolButtonDelete, &QToolButton::clicked, d, &FrameListViewPrivate::onDeleteBtnClicked);
    connect(d->ui->toolButtonExport, &QToolButton::clicked, d, &FrameListViewPrivate::onExportBtnClicked);
    connect(d->ui->toolButtonImport, &QToolButton::clicked, d, &FrameListViewPrivate::onImportBtnClicked);
    connect(d->ui->lineEdit, &QLineEdit::textChanged, d, &FrameListViewPrivate::onFilterText);
    connect(d->ui->tableView, &QTableView::doubleClicked, d, &FrameListViewPrivate::onViewDoubleClicked);
    // clang-format on

    QHeaderView *hHeader = d->ui->tableView->horizontalHeader();
    hHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    hHeader->setStretchLastSection(true);

    d->m_model = new FrameListModel(this);
    d->m_filter = new FrameListFilter(this);
    d->m_filter->setSourceModel(d->m_model);
    d->ui->tableView->setModel(d->m_filter);
}

FrameListView::~FrameListView()
{
    delete d->ui;
}

QJsonObject FrameListView::save() const
{
    QJsonObject obj;
    FrameListViewParameterKeys keys;
    QList<FrameItem> items = d->m_model->frameItems();
    QJsonArray frameArray;
    for (const FrameItem &item : items) {
        QJsonObject obj = frameItemToJson(item);
        frameArray.append(obj);
    }
    obj[keys.frameItems] = frameArray;
    return obj;
}

void FrameListView::load(const QJsonObject &obj)
{
    FrameListViewParameterKeys keys;
    QJsonArray frameArray = obj[keys.frameItems].toArray();
    for (const QJsonValue &value : std::as_const(frameArray)) {
        if (value.isObject()) {
            FrameItem item = frameItemFromJson(value.toObject());
            d->m_model->addFrameItem(item);
        }
    }
}

} // namespace xCanBus