/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKToolStringAssistant.hh"
#include "ui_SAKToolStringAssistant.h"

SAKToolStringAssistant::SAKToolStringAssistant(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKToolStringAssistant)
{
    ui->setupUi(this);
    SAKCommonDataStructure::setComboBoxTextInputFormat(ui->outputFormatComboBox);
    SAKCommonDataStructure::setComboBoxTextOutputFormat(ui->inputFormatComboBox);
}

SAKToolStringAssistant::~SAKToolStringAssistant()
{
    delete ui;
}
