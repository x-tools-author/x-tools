/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luapanel.h"
#include "ui_luapanel.h"

LuaPanel::LuaPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::LuaPanel)
{
    ui->setupUi(this);
}

LuaPanel::~LuaPanel()
{
    delete ui;
}

QVariantMap LuaPanel::save() const
{
    QVariantMap map = Panel::save();
    // Add LuaPanel specific data to the map
    return map;
}

void LuaPanel::load(const QVariantMap &parameters)
{
    Panel::load(parameters);
    // Load LuaPanel specific data from the map
}
