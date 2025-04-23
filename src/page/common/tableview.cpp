/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tableview.h"
#include "ui_tableview.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include "page/utilities/textitemeditor.h"
#include "tablemodel.h"

TableView::TableView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableView)
    , m_editor(new TextItemEditor(this))
{
    ui->setupUi(this);

    QHeaderView *vHeaderView = ui->tableView->verticalHeader();
    vHeaderView->hide();

    // clang-format off
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &TableView::onPushButtonClearClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &TableView::onPushButtonDeleteClicked);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &TableView::onPushButtonImportClicked);
    connect(ui->pushButtonExport, &QPushButton::clicked, this, &TableView::onPushButtonExportClicked);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &TableView::onPushButtonAddClicked);
    // clang-format on

    m_editor = new TextItemEditor(this);
    m_editor->setMinimumWidth(700);
    m_editor->hide();
}

TableView::~TableView()
{
    delete ui;
}

bool TableView::isDisableAll() const
{
    return ui->checkBoxDisable->isChecked();
}

void TableView::setIsDisableAll(bool disable)
{
    ui->checkBoxDisable->setChecked(disable);
}

void TableView::setIdDisableCheckBoxVisible(bool visible)
{
    ui->checkBoxDisable->setVisible(visible);
}

TableModel *TableView::tableModel()
{
    return m_model;
}

void TableView::setTableModel(TableModel *model)
{
    m_model = model;
    ui->tableView->setModel(model);
    connect(ui->tableView, &QTableView::doubleClicked, this, &TableView::onCellDoubleClicked);
}

QTableView *TableView::tableView() const
{
    return ui->tableView;
}

QVariantMap TableView::save() const
{
    QJsonArray array;
    int rows = m_model->rowCount();
    for (int i = 0; i < rows; i++) {
        QVariantMap rawItem = m_model->saveRow(i);
        QJsonObject item = QJsonObject::fromVariantMap(rawItem);
        array.append(item);
    }

    QVariantMap map;
    map.insert("disableAll", isDisableAll());
    map.insert("items", array);
    return map;
}

void TableView::load(const QVariantMap &parameters)
{
    setIsDisableAll(parameters.value("disableAll").toBool());
    auto items = parameters.value("items").toJsonArray();
    if (items.isEmpty()) {
        return;
    }

    m_model->removeRows(0, m_model->rowCount());
    for (int i = 0; i < items.size(); i++) {
        auto item = items.at(i).toObject();
        m_model->insertRows(i, 1);
        m_model->loadRow(i, item.toVariantMap());
    }
}

void TableView::inputBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes);
}

void TableView::onPushButtonClearClicked()
{
    if (!m_model) {
        return;
    }

    int count = m_model->rowCount();
    auto ret = QMessageBox::warning(this,
                                    tr("Clear"),
                                    tr("Are you sure to clear all items?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        m_model->removeRows(0, count);
    }
}

void TableView::onPushButtonDeleteClicked()
{
    if (!m_model) {
        return;
    }

    auto index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, tr("No Item be Selected"), tr("Please select a item first!"));
        return;
    }

    m_model->removeRows(index.row(), 1);
}

void TableView::onPushButtonImportClicked()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    if (!m_model) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Can not open file:" << file.errorString();
        return;
    }

    QByteArray json = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    load(obj.toVariantMap());
}

void TableView::onPushButtonExportClicked()
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

void TableView::onPushButtonAddClicked()
{
    if (!m_model) {
        return;
    }

    m_model->insertRows(m_model->rowCount(), 1);
}

void TableView::onCellDoubleClicked(const QModelIndex &index)
{
    int column = index.column();
    auto universalColumns = textItemColumns();
    if (!universalColumns.contains(column)) {
        return;
    }

    if (!m_model) {
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

QList<int> TableView::textItemColumns() const
{
    return QList<int>();
}
