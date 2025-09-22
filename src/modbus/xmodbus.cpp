/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xmodbus.h"
#include "ui_xmodbus.h"

namespace xModbus {
xModbus::xModbus(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xModbus)
{
    ui->setupUi(this);
}

xModbus::~xModbus()
{
    delete ui;
}
} // namespace xModbus
