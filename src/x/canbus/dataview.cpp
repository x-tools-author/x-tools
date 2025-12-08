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

#include <QMessageBox>

#include "common/xtools.h"
#include "utilities/iconengine.h"

#include "datafilter.h"
#include "datamodel.h"

namespace xCanBus {

struct DataViewParameterKeys
{
    const QString scrollToBottom{"scrollToBottom"};
};

DataView::DataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataView)
{
    ui->setupUi(this);
    ui->toolButtonClear->setIcon(xIcon(":res/icons/mop.svg"));
    ui->toolButtonScrolling->setIcon(xIcon(":res/icons/wrap_text.svg"));
    m_model = new DataModel(this);
    m_filter = new DataFilter(this);
    m_filter->setSourceModel(m_model);
    ui->tableView->setModel(m_filter);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(60);
    xSetNoneBorder(ui->tableView);

    connect(ui->toolButtonClear, &QToolButton::clicked, this, &DataView::onClearBtnClicked);
    connect(m_model, &DataModel::rowsInserted, this, &DataView::onRowInserted);
}

DataView::~DataView()
{
    delete ui;
}

QJsonObject DataView::save()
{
    QJsonObject obj;
    const DataViewParameterKeys keys;
    obj[keys.scrollToBottom] = ui->toolButtonScrolling->isChecked();
    return obj;
}

void DataView::load(const QJsonObject &obj)
{
    const DataViewParameterKeys keys;
    ui->toolButtonScrolling->setChecked(obj.value(keys.scrollToBottom).toBool(true));
}

DataModel *DataView::model()
{
    return m_model;
}

void DataView::onClearBtnClicked()
{
    int ret = QMessageBox::question(this, tr("Question"), tr("Are you sure to clear all data?"));
    if (ret != QMessageBox::Yes) {
        return;
    }

    m_model->clear();
}

void DataView::onRowInserted()
{
    if (ui->toolButtonScrolling->isChecked()) {
        ui->tableView->scrollToBottom();
    }
}

} // namespace xCanBus