/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luaview.h"
#include "ui_luaview.h"

LuaView::LuaView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LuaView)
{
    ui->setupUi(this);
}

LuaView::~LuaView()
{
    delete ui;
}