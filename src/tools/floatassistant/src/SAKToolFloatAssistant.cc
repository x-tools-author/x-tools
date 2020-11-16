/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKToolFloatAssistant.hh"
#include "ui_SAKToolFloatAssistant.h"

SAKToolFloatAssistant::SAKToolFloatAssistant(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKToolFloatAssistant)
{
    ui->setupUi(this);
}

SAKToolFloatAssistant::~SAKToolFloatAssistant()
{
    delete ui;
}
