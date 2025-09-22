/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xmqtt.h"
#include "ui_xmqtt.h"

namespace xMqtt {

xMqtt::xMqtt(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xMqtt)
{
    ui->setupUi(this);
}

xMqtt::~xMqtt()
{
    delete ui;
}
} // namespace xMqtt