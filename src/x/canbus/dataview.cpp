/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "dataview.h"
#include "ui_dataview.h"

#include "datafilter.h"
#include "datamodel.h"

namespace xCanBus {

DataView::DataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataView)
{
    ui->setupUi(this);
    m_model = new DataModel(this);
    m_filter = new DataFilter(this);
    m_filter->setSourceModel(m_model);
    ui->tableView->setModel(m_filter);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(60);
}

DataView::~DataView()
{
    delete ui;
}

} // namespace xCanBus