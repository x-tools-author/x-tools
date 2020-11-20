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

void SAKToolStringAssistant::on_textEdit_textChanged()
{

}

void SAKToolStringAssistant::on_inputFormatComboBox_currentIndexChanged(int index)
{

}

void SAKToolStringAssistant::on_hexRawDataCheckBox_clicked()
{

}

void SAKToolStringAssistant::on_outputFormatComboBox_activated(const QString &arg1)
{

}

void SAKToolStringAssistant::on_createPushButton_clicked()
{

}
