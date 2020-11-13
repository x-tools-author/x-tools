/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonRegisterViewController.hh"
#include "ui_SAKModbusCommonRegisterViewController.h"

SAKModbusCommonRegisterViewController::SAKModbusCommonRegisterViewController(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonRegisterViewController)
{
    ui->setupUi(this);
}

SAKModbusCommonRegisterViewController::~SAKModbusCommonRegisterViewController()
{
    delete ui;
}

void SAKModbusCommonRegisterViewController::on_updatePushButton_clicked()
{
    int startAddress = ui->startAddressSpinBox->value();
    int registerNumber = ui->registerNumberSpinBox->value();
    emit inVokeUpdateRegister(startAddress, registerNumber);
}
