/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregistereditor.h"
#include "ui_modbusregistereditor.h"

namespace xModbus {

ModbusRegisterEditorParameters ModbusRegisterEditor::s_ctx{1, 10, 1};
ModbusRegisterEditor::ModbusRegisterEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModbusRegisterEditor)
{
    ui->setupUi(this);

    ui->spinBoxStartAddress->setValue(s_ctx.startAddress);
    ui->spinBoxQuantity->setValue(s_ctx.quantity);
    ui->spinBoxServerAddress->setValue(s_ctx.serverAddress);
}

ModbusRegisterEditor::~ModbusRegisterEditor()
{
    cacheParameters();
    delete ui;
}

ModbusRegisterEditorParameters ModbusRegisterEditor::parameters() const
{
    cacheParameters();
    return s_ctx;
}

void ModbusRegisterEditor::cacheParameters() const
{
    s_ctx.startAddress = ui->spinBoxStartAddress->value();
    s_ctx.quantity = ui->spinBoxQuantity->value();
    s_ctx.serverAddress = ui->spinBoxServerAddress->value();
}

} // namespace xModbus