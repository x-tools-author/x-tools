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
class ModbusRegisterGroupEditor;
}

namespace xModbus {

struct ModbusRegisterGroupEditorParameters
{
    QString name;
    QModbusDataUnit::RegisterType type{QModbusDataUnit::Invalid};
    int startAddress;
    int quantity;
    int serverAddress;
};

class ModbusRegisterGroupEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ModbusRegisterGroupEditor(QWidget *parent = nullptr);
    ~ModbusRegisterGroupEditor() override;

    ModbusRegisterGroupEditorParameters parameters() const;

private:
    Ui::ModbusRegisterGroupEditor *ui;
    static ModbusRegisterGroupEditorParameters s_ctx;

private:
    void cacheParameters() const;
};

} // namespace xModbus