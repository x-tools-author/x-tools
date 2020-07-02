/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */

#include "SAKInputCrcSettingsDialog.hh"
#include "ui_SAKInputCrcSettingDialog.h"

SAKInputCrcSettingsDialog::SAKInputCrcSettingsDialog(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKInputCrcSettingDialog)
{
    bigEndianCheckBox = ui->bigEndianCheckBox;
    startSpinBox = ui->startSpinBox;
    endSpinBox = ui->endSpinBox;
}

SAKInputCrcSettingsDialog::~SAKInputCrcSettingsDialog()
{
    delete ui;
}

SAKInputCrcSettingsDialog::ParameterContext SAKInputCrcSettingsDialog::parametersContext()
{
    SAKInputCrcSettingsDialog::ParameterContext ctx;
    parameterCtxMutex.lock();
    ctx.endByte = parameterCtx.endByte;
    ctx.startByte = parameterCtx.startByte;
    ctx.isBigEndianCRC = parameterCtx.isBigEndianCRC;
    parameterCtxMutex.unlock();
    return ctx;
}

void SAKInputCrcSettingsDialog::on_bigEndianCheckBox_clicked()
{
    parameterCtxMutex.lock();
    parameterCtx.isBigEndianCRC = bigEndianCheckBox->isChecked();
    parameterCtxMutex.unlock();
}

void SAKInputCrcSettingsDialog::on_startSpinBox_valueChanged(int value)
{
    parameterCtxMutex.lock();
    parameterCtx.startByte = value;
    parameterCtxMutex.unlock();
}

void SAKInputCrcSettingsDialog::on_endSpinBox_valueChanged(int value)
{
    parameterCtxMutex.lock();
    parameterCtx.endByte = value;
    parameterCtxMutex.unlock();
}
