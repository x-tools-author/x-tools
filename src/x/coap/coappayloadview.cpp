/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadview.h"
#include "ui_coappayloadview.h"

#include <QAction>
#include <QActionGroup>
#include <QHeaderView>
#include <QJsonArray>
#include <QMenu>
#include <QMessageBox>

#include "utilities/compatibility.h"
#include "utilities/iconengine.h"

#include "coapcommon.h"
#include "coappayloadeditor.h"
#include "coappayloadfilter.h"
#include "coappayloadmodel.h"

namespace Ui {
class CoAPPayloadView;
};

namespace xCoAP {

class CoAPPayloadViewPrivate : public QObject
{
public:
    CoAPPayloadViewPrivate(CoAPPayloadView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPPayloadView();
        ui->setupUi(q);
        ui->toolButtonColumns->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->toolButtonColumns->setIcon(xIcon(":res/icons/list.svg"));
        ui->toolButtonColumns->hide();
        ui->toolButtonAdd->setIcon(xIcon(":res/icons/add.svg"));
        ui->toolButtonRemove->setIcon(xIcon(":res/icons/delete_sweep.svg"));
        ui->toolButtonEdit->setIcon(xIcon(":res/icons/edit_note.svg"));
        ui->toolButtonClear->setIcon(xIcon(":res/icons/delete.svg"));
        ui->toolButtonEdit->hide();

        // Model
        m_model = new CoAPPayloadModel(q);
        m_filter = new CoAPPayloadFilter(q);
        m_filter->setSourceModel(m_model);
        ui->tableView->setModel(m_filter);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        connect(m_model, &CoAPPayloadModel::dataChanged, q, [=]() { onDataChanged(); });
        connect(ui->tableView, &QTableView::doubleClicked, q, [=](const QModelIndex& index) {
            onDoubleClicked(index);
        });
        connect(ui->comboBoxFormats, xComboBoxIndexChanged, q, [=]() { onFormatChanged(); });
        connect(ui->lineEditFilter, &QLineEdit::textChanged, q, [=]() { onFilterTextChanged(); });
        connect(ui->toolButtonClear, &QToolButton::clicked, q, [=]() { onClear(); });
        connect(ui->toolButtonAdd, &QToolButton::clicked, q, [this]() { onAdd(); });
        connect(ui->toolButtonEdit, &QToolButton::clicked, q, [this]() { onEdit(); });
        connect(ui->toolButtonRemove, &QToolButton::clicked, q, [this]() { onRemove(); });

        // Menus
        m_toolButtonMenu = new QMenu(q);
        CoAPCommon::setupContextFormat(ui->comboBoxFormats);
        ui->comboBoxFormats->insertItem(0, tr("All formats"), -1);
        int columns = m_model->columnCount(QModelIndex());
        m_columnsMenu = new QMenu(q);
        for (int i = 0; i < columns; ++i) {
            QString colName = m_model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
            QAction* action = new QAction(colName);
            action->setCheckable(true);
            action->setChecked(true);
            action->setData(i);
            m_columnsMenu->addAction(action);
            QObject::connect(action, &QAction::toggled, q, [=](bool checked) {
                ui->tableView->setColumnHidden(i, !checked);
            });
        }
        ui->toolButtonColumns->setMenu(m_columnsMenu);
        ui->toolButtonColumns->setPopupMode(QToolButton::MenuButtonPopup);

        // Payload editor
        m_editor = new CoAPPayloadEditor();
        connect(m_editor, &CoAPPayloadEditor::accepted, q, [this]() { onAccepted(); });
    }
    ~CoAPPayloadViewPrivate() override { m_editor->deleteLater(); }

public:
    Ui::CoAPPayloadView* ui{nullptr};
    CoAPPayloadModel* m_model{nullptr};
    CoAPPayloadFilter* m_filter{nullptr};

    QMenu* m_columnsMenu{nullptr};
    QMenu* m_toolButtonMenu{nullptr};

public:
    void onDataChanged()
    {
        m_toolButtonMenu->clear();
        for (int row = 0; row < m_model->rowCount(QModelIndex()); ++row) {
            QString desc = m_model->data(m_model->index(row, 0), Qt::DisplayRole).toString();
            QAction* action = m_toolButtonMenu->addAction(desc);
            action->setData(row);

            connect(action, &QAction::triggered, q, [=]() {
                QVariant obj = m_model->data(m_model->index(row, 0),
                                             CO_AP_PAYLOAD_DATA_ROLE_PAYLOAD);
                if (!obj.isValid()) {
                    return;
                }

                auto payloadPtr = obj.value<std::shared_ptr<CoAPCommon::PayloadContext>>();
                if (!payloadPtr) {
                    return;
                }
                CoAPCommon::PayloadContext ctx = *payloadPtr;
                emit q->payloadActivated(ctx.format, ctx.data);
            });
        }
    }
    void onFormatChanged()
    {
        const int format = ui->comboBoxFormats->currentData().toInt();
        m_filter->setFormat(format);
    }
    void onFilterTextChanged()
    {
        const QString text = ui->lineEditFilter->text().trimmed();
        m_filter->setFilterText(text);
    }
    void onClear()
    {
        int ret = QMessageBox::question(q,
                                        QObject::tr("Clear all payloads"),
                                        QObject::tr("Are you sure to clear all payloads?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            this->m_model->clearPayloads();
        }
    }
    void onAdd()
    {
        CoAPCommon::PayloadContext ctx = CoAPCommon::defaultPayloadContext();
        m_editor->load(CoAPCommon::payloadContext2JsonObject(ctx));
        m_editor->show();
    }
    void onEdit() { m_editor->show(); }
    void onRemove()
    {
        if (!CoAPCommon::deleteSelectedItems()) {
            return;
        }

        QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            qCDebug(xCoAPClientLog) << "No selected indexes to remove.";
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
            m_model->removeRow(rowList.first());
        }
    }
    void onDoubleClicked(const QModelIndex& index)
    {
        QModelIndex sourceIndex = m_filter->mapToSource(index);
        QVariant obj = m_model->data(sourceIndex, CO_AP_PAYLOAD_DATA_ROLE_PAYLOAD);
        if (!obj.isValid()) {
            qCDebug(xCoAPClientLog) << "Invalid payload context.";
            return;
        }

        auto payloadPtr = obj.value<std::shared_ptr<CoAPCommon::PayloadContext>>();
        if (!payloadPtr) {
            qCDebug(xCoAPClientLog) << "Null payload context.";
            return;
        }

        qCDebug(xCoAPClientLog) << "Editing payload at row:" << sourceIndex.row();
        m_editor->load(CoAPCommon::payloadContext2JsonObject(*payloadPtr));
        m_editor->setFromRow(sourceIndex.row());
        m_editor->show();
    }
    void onAccepted()
    {
        QJsonObject obj = m_editor->save();
        m_editor->close();
        if (m_editor->fromRow() >= 0) {
            m_model->updateRow(m_editor->fromRow(), obj);
        } else {
            m_model->addPayload(obj);
        }

        m_editor->setFromRow(-1);
    }

private:
    CoAPPayloadView* q{nullptr};
    CoAPPayloadEditor* m_editor{nullptr};
};

struct CoAPPayloadViewParameterKeys
{
    const QString columns{"columns"};
    const QString payloads{"payloads"};
    const QString format{"format"};
    const QString filterText{"filterText"};
};

CoAPPayloadView::CoAPPayloadView(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPPayloadViewPrivate(this);
}

CoAPPayloadView::~CoAPPayloadView() {}

QJsonObject CoAPPayloadView::save()
{
    CoAPPayloadViewParameterKeys keys;
    QJsonObject obj;
    QJsonArray columnsArray;
    int columnCount = d->m_model->columnCount(QModelIndex());
    for (int i = 0; i < columnCount; ++i) {
        bool isVisible = !d->ui->tableView->isColumnHidden(i);
        columnsArray.append(isVisible);
    }
    obj[keys.columns] = columnsArray;

    // Payloads
    QJsonArray payloadsArray;
    for (int row = 0; row < d->m_model->rowCount(QModelIndex()); ++row) {
        QVariant ctx = d->m_model->data(d->m_model->index(row, 0), CO_AP_PAYLOAD_DATA_ROLE_PAYLOAD);
        if (!ctx.isValid()) {
            continue;
        }

        auto payloadPtr = ctx.value<std::shared_ptr<CoAPCommon::PayloadContext>>();
        if (!payloadPtr) {
            continue;
        }

        QJsonObject payloadObj = CoAPCommon::payloadContext2JsonObject(*payloadPtr);
        ;
        payloadsArray.append(payloadObj);
    }
    obj[keys.payloads] = payloadsArray;
    obj[keys.format] = d->ui->comboBoxFormats->currentData().toInt();
    obj[keys.filterText] = d->ui->lineEditFilter->text().trimmed();

    return obj;
}

void CoAPPayloadView::load(const QJsonObject& obj)
{
    CoAPPayloadViewParameterKeys keys;
    QJsonArray columnsArray = obj.value(keys.columns).toArray();
    int columnCount = d->m_model->columnCount(QModelIndex());
    QList<QAction*> actions = d->m_columnsMenu->actions();
    for (int i = 0; i < columnCount && i < columnsArray.size() && i < actions.size(); ++i) {
        bool isVisible = columnsArray.at(i).toBool(true);
        d->ui->tableView->setColumnHidden(i, !isVisible);
        actions.at(i)->setChecked(isVisible);
    }

    // Payloads
    QJsonArray payloadsArray = obj.value(keys.payloads).toArray();
    for (const QJsonValue& val : std::as_const(payloadsArray)) {
        QJsonObject payloadObj = val.toObject();
        d->m_model->addPayload(payloadObj);
    }

    int index = d->ui->comboBoxFormats->findData(obj.value(keys.format).toInt(0));
    d->ui->comboBoxFormats->setCurrentIndex(index >= 0 ? index : 0);
    d->ui->lineEditFilter->setText(obj.value(keys.filterText).toString());
    d->m_filter->setFormat(obj.value(keys.format).toInt());
    d->m_filter->setFilterText(obj.value(keys.filterText).toString());
}

QMenu* CoAPPayloadView::toolButtonMenu() const
{
    return d->m_toolButtonMenu;
}

QString CoAPPayloadView::addPayload(const QString& description,
                                    int contextFormat,
                                    const QByteArray& payload)
{
    CoAPCommon::PayloadContext ctx;
    ctx.description = description;
    ctx.format = contextFormat;
    ctx.data = payload;
    QJsonObject obj = CoAPCommon::payloadContext2JsonObject(ctx);
    d->m_model->addPayload(obj);
    return QString("");
}

} // namespace xCoAP
