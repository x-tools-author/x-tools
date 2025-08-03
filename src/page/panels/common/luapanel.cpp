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

#include <QFile>

LuaPanel::LuaPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::LuaPanel)
    , m_menu(nullptr)
{
    ui->setupUi(this);

    m_menu = new QMenu(this);
    m_menu->addAction(tr("Default Lua Script"), this, &LuaPanel::onDefaultLuaScriptTriggered);
    m_menu->addAction(tr("Checksum Lua Script"), this, &LuaPanel::onCheckSumLuaScriptTriggered);
    onDefaultLuaScriptTriggered();
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

QMenu *LuaPanel::buttonMenu() const
{
    return m_menu;
}

void LuaPanel::onDefaultLuaScriptTriggered()
{
    QFile file(":/res/scripts/lua/default.lua");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString script = file.readAll();
        ui->plainTextEditScript->setPlainText(script);
        file.close();
    }
}

void LuaPanel::onCheckSumLuaScriptTriggered()
{
    QFile file(":/res/scripts/lua/check_sum.lua");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString script = file.readAll();
        ui->plainTextEditScript->setPlainText(script);
        file.close();
    }
}