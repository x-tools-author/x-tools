/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xopcua.h"
#include "ui_xopcua.h"

namespace xOpcUa {

xOpcUa::xOpcUa(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xOpcUa)
{
    ui->setupUi(this);
}

xOpcUa::~xOpcUa()
{
    delete ui;
}

QJsonObject xOpcUa::save()
{
    QJsonObject obj;
    return obj;
}

void xOpcUa::load(const QJsonObject& obj)
{
    Q_UNUSED(obj);
}

} // namespace xOpcUa