/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace xModbus {

class ModbusRegisterTable;
class RegisterView : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterView(QWidget *parent = nullptr);
    ~RegisterView() override;

    ModbusRegisterTable *registerTable() const;

private:
    ModbusRegisterTable *m_registerTable{nullptr};
};

} // namespace xModbus