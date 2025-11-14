/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregistergroupeditor.h"
#include "ui_modbusregistergroupeditor.h"

#include "modbuscommon.h"

namespace xModbus {

ModbusRegisterGroupEditorParameters ModbusRegisterGroupEditor::s_ctx{QString("New Register Group"),
                                                                     xCoils,
                                                                     0,
                                                                     10,
                                                                     1};
ModbusRegisterGroupEditor::ModbusRegisterGroupEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModbusRegisterGroupEditor)
{
    ui->setupUi(this);
    setupRegisterType(ui->comboBoxType);

    int index = ui->comboBoxType->findData(int(s_ctx.type));
    ui->comboBoxType->setCurrentIndex(index < 0 ? 0 : index);
    ui->lineEditName->setText(s_ctx.name);
    ui->spinBoxStartAddress->setValue(s_ctx.startAddress);
    ui->spinBoxQuantity->setValue(s_ctx.quantity);
    ui->spinBoxServerAddress->setValue(s_ctx.serverAddress);
}

ModbusRegisterGroupEditor::~ModbusRegisterGroupEditor()
{
    cacheParameters();
    delete ui;
}

ModbusRegisterGroupEditorParameters ModbusRegisterGroupEditor::parameters() const
{
    cacheParameters();
    return s_ctx;
}

void ModbusRegisterGroupEditor::cacheParameters() const
{
    int type = ui->comboBoxType->currentData().toInt();
    s_ctx.name = ui->lineEditName->text();
    s_ctx.type = static_cast<QModbusDataUnit::RegisterType>(type);
    s_ctx.startAddress = ui->spinBoxStartAddress->value();
    s_ctx.quantity = ui->spinBoxQuantity->value();
    s_ctx.serverAddress = ui->spinBoxServerAddress->value();
}

} // namespace xModbus