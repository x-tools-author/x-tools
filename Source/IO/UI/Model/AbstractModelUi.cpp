/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AbstractModelUi.h"
#include "ui_AbstractModelUi.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardItemModel>

#include "../../IO/Model/AbstractModelIO.h"
#include "../../Unit/DataEditor.h"

namespace xTools {

AbstractModelUi::AbstractModelUi(QWidget *parent)
    : AbstractIOUi{parent}
    , ui(new Ui::AbstractModelUi)
{
    ui->setupUi(this);
#if 0
    ui->tableView->setEditTriggers(QTableView::EditTrigger::DoubleClicked);
#else
    connect(ui->tableView, &QTableView::doubleClicked, this, &AbstractModelUi::onCellDoubleClicked);
#endif

    QHeaderView *vHeaderView = ui->tableView->verticalHeader();
    vHeaderView->hide();

    connect(ui->pushButtonClear,
            &QPushButton::clicked,
            this,
            &AbstractModelUi::onPushButtonClearClicked);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &AbstractModelUi::onPushButtonDeleteClicked);
    connect(ui->pushButtonImport,
            &QPushButton::clicked,
            this,
            &AbstractModelUi::onPushButtonImportClicked);
    connect(ui->pushButtonExport,
            &QPushButton::clicked,
            this,
            &AbstractModelUi::onPushButtonExportClicked);
    connect(ui->pushButtonAdd,
            &QPushButton::clicked,
            this,
            &AbstractModelUi::onPushButtonAddClicked);

    m_editor = new DataEditor(this);
    m_editor->setMinimumWidth(700);
    m_editor->hide();
}

AbstractModelUi::~AbstractModelUi()
{
    delete ui;
}

QTableView *AbstractModelUi::tableView() const
{
    return ui->tableView;
}

QVariantMap AbstractModelUi::save() const
{
    QJsonArray array;
    int rows = m_model->rowCount();
    for (int i = 0; i < rows; i++) {
        QVariantMap rawItem = m_io->saveItem(i);
        QJsonObject item = QJsonObject::fromVariantMap(rawItem);
        array.append(item);
    }

    QVariantMap map;
    map.insert("items", array);
    return map;
}

void AbstractModelUi::load(const QVariantMap &parameters)
{
    auto items = parameters.value("items").toJsonArray();
    if (items.isEmpty()) {
        return;
    }

    m_model->removeColumns(0, m_model->rowCount());
    for (int i = 0; i < items.size(); i++) {
        auto item = items.at(i).toObject();
        m_model->insertRows(i, 1);
        m_io->loadItem(i, item.toVariantMap());
    }
}

void AbstractModelUi::setupIO(AbstractIO *io)
{
    m_io = qobject_cast<AbstractModelIO *>(io);
    if (m_io) {
        auto tableModelVar = m_io->tableModel();
        m_model = qvariant_cast<QAbstractTableModel *>(tableModelVar);
        if (m_model) {
            ui->tableView->setModel(m_model);
            auto delegate = createItemDelegate();
            if (delegate) {
                ui->tableView->setItemDelegate(delegate);
            }
        } else {
            qCritical() << "Can not cast to QAbstractTableModel";
        }
    } else {
        qCritical() << "Can not cast to AbstractModel";
    }
}

QList<int> AbstractModelUi::textItemColumns() const
{
    return QList<int>();
}

QStyledItemDelegate *AbstractModelUi::createItemDelegate() const
{
    return nullptr;
}

void AbstractModelUi::onPushButtonClearClicked()
{
    int count = m_model->rowCount();
    auto ret = QMessageBox::warning(this,
                                    tr("Clear"),
                                    tr("Are you sure to clear all items?").arg(count),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        m_model->removeRows(0, count);
    }
}

void AbstractModelUi::onPushButtonDeleteClicked()
{
    auto index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, tr("No Item be Selected"), tr("Please select a item first!"));
        return;
    }

    m_model->removeRows(index.row(), 1);
}

void AbstractModelUi::onPushButtonImportClicked()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QByteArray json = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QJsonArray array = doc.array();
        for (int i = 0; i < json.size(); i++) {
            auto obj = array.at(i).toObject();
            m_model->insertRows(m_model->rowCount(), 1);
        }
        file.close();
    } else {
        qWarning() << "Can not open file:" << file.errorString();
    }
}

void AbstractModelUi::onPushButtonExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QTextStream out(&file);
        QVariantMap map = save();
        QJsonDocument doc = QJsonDocument::fromVariant(map);
        out << doc.toJson();
        file.close();
    } else {
        qWarning() << "Can not open file:" << file.errorString();
    }
}

void AbstractModelUi::onPushButtonAddClicked()
{
    m_model->insertRows(m_model->rowCount(), 1);
}

void AbstractModelUi::onCellDoubleClicked(const QModelIndex &index)
{
    int column = index.column();
    auto universalColumns = textItemColumns();
    if (!universalColumns.contains(column)) {
        return;
    }

    auto parameters = m_model->data(index, Qt::EditRole);
    m_editor->load(parameters.toJsonObject());
    auto ret = m_editor->exec();
    if (ret == QDialog::Accepted) {
        auto parameters = m_editor->save();
        m_model->setData(index, parameters, Qt::EditRole);
    }
}

} // namespace xTools
