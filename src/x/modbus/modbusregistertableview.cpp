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
}

ModbusRegisterTableView::~ModbusRegisterTableView()
{
    delete ui;
}

ModbusRegisterTable *ModbusRegisterTableView::registerTable() const
{
    return m_registerTable;
}

} // namespace xModbus