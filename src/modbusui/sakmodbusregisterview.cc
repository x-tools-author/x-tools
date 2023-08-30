/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "sakmodbusregisterview.h"
#include "ui_sakmodbusregisterview.h"

SAKModbusRegisterView::SAKModbusRegisterView(QWidget *parent)
    : QWidget{parent}
    , ui_(new Ui::SAKModbusRegisterView) {
    ui_->setupUi(this);

    QStringList labels;
    labels << tr("Address") << tr("Value") << tr("Description");
    ui_->register_table_widget_->setColumnCount(3);
    ui_->register_table_widget_->setHorizontalHeaderLabels(labels);
    QHeaderView *h_view = ui_->register_table_widget_->horizontalHeader();
    h_view->setStretchLastSection(true);
    QHeaderView *v_view = ui_->register_table_widget_->verticalHeader();
    v_view->hide();
}

SAKModbusRegisterView::~SAKModbusRegisterView() {
    delete ui_;
}
