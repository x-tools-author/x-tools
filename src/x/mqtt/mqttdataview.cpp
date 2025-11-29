/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttdataview.h"
#include "ui_mqttdataview.h"

#include "mqttdatafilter.h"
#include "mqttdatamodel.h"

namespace xMQTT {

MqttDataView::MqttDataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttDataView)
{
    ui->setupUi(this);
    m_model = new MqttDataModel(this);
    m_filter = new MqttDataFilter(this);
    m_filter->setSourceModel(m_model);
    ui->tableView->setModel(m_filter);
    ui->tableView->setAlternatingRowColors(true);
    QHeaderView *hView = ui->tableView->horizontalHeader();
    hView->setSectionResizeMode(QHeaderView::ResizeToContents);
    hView->setStretchLastSection(true);
    hView->setMinimumSectionSize(80);
}

MqttDataView::~MqttDataView()
{
    delete ui;
}

MqttDataModel *MqttDataView::model() const
{
    return m_model;
}

} // namespace xMQTT