/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDialog>
#include <QModbusDataUnit>

namespace Ui {
class ModbusRegisterEditor;
}

namespace xModbus {

struct ModbusRegisterEditorParameters
{
    QModbusDataUnit::RegisterType type;
    int startAddress;
    int quantity;
    int serverAddress;
};

class ModbusRegisterEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ModbusRegisterEditor(QWidget *parent = nullptr);
    ~ModbusRegisterEditor() override;

    ModbusRegisterEditorParameters parameters() const;

private:
    Ui::ModbusRegisterEditor *ui;
    static ModbusRegisterEditorParameters s_ctx;

private:
    void cacheParameters() const;
};

} // namespace xModbus