/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xftp.h"
#include "ui_xftp.h"

namespace xFtp {

xFtp::xFtp(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xFtp)
{
    ui->setupUi(this);
}

xFtp::~xFtp()
{
    delete ui;
}
} // namespace xFtp