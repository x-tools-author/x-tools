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

namespace Ui {
class ModbusRegisterEditor;
}

namespace xModbus {

class ModbusRegisterEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ModbusRegisterEditor(QWidget *parent = nullptr);
    ~ModbusRegisterEditor() override;

private:
    Ui::ModbusRegisterEditor *ui;
};

} // namespace xModbus