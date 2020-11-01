/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonHostSection.hh"
#include "ui_SAKModbusCommonHostSection.h"

SAKModbusCommonHostSection::SAKModbusCommonHostSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonHostSection)
{
    ui->setupUi(this);
}

SAKModbusCommonHostSection::~SAKModbusCommonHostSection()
{
    delete ui;
}
