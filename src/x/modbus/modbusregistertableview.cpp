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
    ui->tableView->setItemDelegate(m_registerDelegate);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(80);
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
}

} // namespace xModbus