/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xprofibus.h"
#include "ui_xprofibus.h"

namespace xProfibus {

xProfibus::xProfibus(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xProfibus)
{
    ui->setupUi(this);
}

xProfibus::~xProfibus()
{
    delete ui;
}
} // namespace xProfibus