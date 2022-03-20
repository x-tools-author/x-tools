/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include "SAKModbusCommonRegisterViewController.hh"
#include "ui_SAKModbusCommonRegisterViewController.h"

SAKModbusCommonRegisterViewController::SAKModbusCommonRegisterViewController(QModbusDataUnit::RegisterType type, QWidget *parent)
    :QWidget(parent)
    ,mRegisterType(type)
    ,ui(new Ui::SAKModbusCommonRegisterViewController)
{
    ui->setupUi(this);
}

SAKModbusCommonRegisterViewController::~SAKModbusCommonRegisterViewController()
{
    delete ui;
}

QModbusDataUnit::RegisterType SAKModbusCommonRegisterViewController::registerType()
{
    return mRegisterType;
}

void SAKModbusCommonRegisterViewController::on_updatePushButton_clicked()
{
    int startAddress = ui->startAddressSpinBox->value();
    int registerNumber = ui->registerNumberSpinBox->value();
    emit invokeUpdateRegister(startAddress, registerNumber);
}

void SAKModbusCommonRegisterViewController::on_exportPushButton_clicked()
{
    emit invokeExport();
}

void SAKModbusCommonRegisterViewController::on_importPushButton_clicked()
{
    emit invokeImport();
}
