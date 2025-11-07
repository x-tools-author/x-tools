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

#include "common/iconengine.h"
#include "common/menu.h"

#include "modbusregisterdelegate.h"
#include "modbusregistertable.h"
#include "modbusregistertablefilter.h"

namespace xModbus {

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

    connect(ui->lineEditFilter,
            &QLineEdit::textChanged,
            this,
            &ModbusRegisterTableView::onFilterTextChanged);
}

ModbusRegisterTableView::~ModbusRegisterTableView()
{
    delete ui;
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

} // namespace xModbus