/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonFlowLayout.hh"
#include "SAKModbusCommonReigsterView.hh"
#include "ui_SAKModbusCommonReigsterView.h"

SAKModbusCommonReigsterView::SAKModbusCommonReigsterView(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonReigsterView)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(new SAKModbusCommonFlowLayout);
}

SAKModbusCommonReigsterView::~SAKModbusCommonReigsterView()
{
    delete ui;
}

void SAKModbusCommonReigsterView::addWidget(QWidget *registerWisget)
{
    ui->scrollAreaWidgetContents->layout()->addWidget(registerWisget);
}
