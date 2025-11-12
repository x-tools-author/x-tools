/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregistertableview.h"
#include "ui_modbusregistertableview.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextStream>

#include "common/iconengine.h"
#include "common/menu.h"

#include "modbuscommon.h"
#include "modbusregisterdelegate.h"
#include "modbusregistertable.h"
#include "modbusregistertablefilter.h"

namespace xModbus {

struct ModbusRegisterTableViewKeys
{
    const QString registers{"registers"};
    const QString registerTableName{"registerTableName"};
};

ModbusRegisterTableView::ModbusRegisterTableView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusRegisterTableView)
{
    ui->setupUi(this);
    m_registerTable = new ModbusRegisterTable(this);
    m_registerTableFilter = new ModbusRegisterTableFilter(this);
    m_registerDelegate = new ModbusRegisterDelegate(this);
    m_registerTableFilter->setSourceModel(m_registerTable);
    ui->tableView->setModel(m_registerTableFilter);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
#if 0
    ui->tableView->verticalHeader()->setSectionsMovable(true);
#endif
    ui->tableView->setItemDelegate(m_registerDelegate);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(80);
    ui->toolButtonColumns->setIcon(xIcon(":/res/icons/list.svg"));
    ui->toolButtonColumns->setToolTip(ui->toolButtonColumns->text());
#if 1
    ui->toolButtonColumns->setToolButtonStyle(Qt::ToolButtonIconOnly);
#endif

    m_columnMenu = new Menu(ui->toolButtonColumns);
    ui->toolButtonColumns->setMenu(m_columnMenu);
    ui->toolButtonColumns->setPopupMode(QToolButton::InstantPopup);
    resetColumnMenu();

    // clang-format off
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &ModbusRegisterTableView::onFilterTextChanged);
    connect(ui->toolButtonNew, &QToolButton::clicked, this, &ModbusRegisterTableView::onAddRegisterButtonClicked);
    connect(ui->toolButtonDelete, &QToolButton::clicked, this, &ModbusRegisterTableView::onRemoveRegisterButtonClicked);
    connect(ui->toolButtonClear, &QToolButton::clicked, this, &ModbusRegisterTableView::onClearRegistersButtonClicked);
    connect(ui->toolButtonSave, &QToolButton::clicked, this, &ModbusRegisterTableView::onSaveRegistersButtonClicked);
    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &ModbusRegisterTableView::onLoadRegistersButtonClicked);
    // clang-format on
}

ModbusRegisterTableView::~ModbusRegisterTableView()
{
    delete ui;
}

QJsonObject ModbusRegisterTableView::save() const
{
    QJsonObject obj;
    ModbusRegisterTableViewKeys keys;
    obj[keys.registers] = saveRegisters();
    obj[keys.registerTableName] = windowTitle();
    return obj;
}

void ModbusRegisterTableView::load(const QJsonObject &obj)
{
    ModbusRegisterTableViewKeys keys;
    loadRegisters(obj[keys.registers].toArray());
    setWindowTitle(obj[keys.registerTableName].toString());
}

ModbusRegisterTable *ModbusRegisterTableView::registerTable() const
{
    return m_registerTable;
}

void ModbusRegisterTableView::selectRow(int row)
{
    QModelIndex idx = m_registerTableFilter->index(row, 0);
    if (!idx.isValid()) {
        qWarning("ModbusRegisterTableView::selectRow: invalid row %d", row);
        return;
    }

    ui->tableView->clearSelection();
    ui->tableView->setCurrentIndex(idx);
    ui->tableView->scrollTo(idx, QAbstractItemView::PositionAtTop);
}

void ModbusRegisterTableView::setServerAddressColumnVisible(bool visible)
{
    ui->tableView->setColumnHidden(REGISTER_TABLE_SERVER_ADDRESS, !visible);
    resetColumnMenu();
}

void ModbusRegisterTableView::onFilterTextChanged(const QString &text)
{
    m_registerTableFilter->setFilterFixedString(text);
}

void ModbusRegisterTableView::onAddRegisterButtonClicked()
{
    int row = m_registerTable->rowCount(QModelIndex());
    m_registerTable->insertRow(row);
}

void ModbusRegisterTableView::onRemoveRegisterButtonClicked()
{
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        showEmptySelectedItemWarning();
        return;
    }

    // Remove from the last selected row to avoid shifting issues
    std::sort(selectedIndexes.begin(),
              selectedIndexes.end(),
              [](const QModelIndex &a, const QModelIndex &b) { return a.row() > b.row(); });

    for (const QModelIndex &index : selectedIndexes) {
        QModelIndex sourceIndex = m_registerTableFilter->mapToSource(index);
        m_registerTable->removeRow(sourceIndex.row());
    }
}

void ModbusRegisterTableView::onClearRegistersButtonClicked()
{
    int ret = showClearViewDataWarning();
    if (ret != QMessageBox::Yes) {
        return;
    }

    m_registerTable->removeRows(0, m_registerTable->rowCount(QModelIndex()), QModelIndex());
}

void ModbusRegisterTableView::onSaveRegistersButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Registers"),
                                                    QString(),
                                                    tr("json (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    // Save the register data to the specified file
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file for saving."));
        return;
    }

    QJsonArray jsonArray = saveRegisters();
    QJsonDocument jsonDoc(jsonArray);
    QTextStream out(&file);
    out << jsonDoc.toJson(QJsonDocument::Indented);
    file.close();
}

void ModbusRegisterTableView::onLoadRegistersButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Registers"),
                                                    QString(),
                                                    tr("json (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    // Load the register data from the specified file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file for loading."));
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (!jsonDoc.isArray()) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid JSON format in the file."));
        return;
    }

    int rowCount = m_registerTable->rowCount(QModelIndex());
    if (rowCount > 0) {
        int ret = QMessageBox::question(
            this,
            tr("Import Registers"),
            tr("The register table is not empty. Do you want to merge existing registers?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
        if (ret == QMessageBox::No) {
            m_registerTable->removeRows(0, rowCount, QModelIndex());
            return;
        }
    }

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            ModbusRegister *reg = new ModbusRegister(this);
            reg->load(value.toObject());
            m_registerTable->addRegisterItem(reg);
        }
    }
}

void ModbusRegisterTableView::resetColumnMenu()
{
    m_columnMenu->clear();
    int columns = ui->tableView->model()->columnCount();
    for (int col = 0; col < columns; ++col) {
        QString header = ui->tableView->model()->headerData(col, Qt::Horizontal).toString();
        QAction *action = m_columnMenu->addAction(header);
        action->setCheckable(true);
        action->setChecked(!ui->tableView->isColumnHidden(col));
        connect(action, &QAction::toggled, this, [this, col](bool checked) {
            ui->tableView->setColumnHidden(col, !checked);
        });
    }
}

QJsonArray ModbusRegisterTableView::saveRegisters() const
{
    QJsonArray array;
    QList<ModbusRegister *> registerItems = m_registerTable->registerItems();
    for (ModbusRegister *reg : registerItems) {
        array.append(reg->save());
    }

    return array;
}

void ModbusRegisterTableView::loadRegisters(const QJsonArray &array)
{
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            ModbusRegister *reg = new ModbusRegister(this);
            reg->load(value.toObject());
            m_registerTable->addRegisterItem(reg);
        }
    }
}

} // namespace xModbus