/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptsmanager.h"
#include "ui_scriptsmanager.h"

#include "scriptjs.h"
#include "scriptlua.h"

ScriptsManager::ScriptsManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScriptsManager)
{
    ui->setupUi(this);
    m_lua = new ScriptLua(this);
    m_js = new ScriptJs(this);
    ui->tabWidget->addTab(m_lua, tr("Lua"));
    ui->tabWidget->addTab(m_js, tr("JavaScript"));
}

ScriptsManager::~ScriptsManager()
{
    delete ui;
}

QJsonObject ScriptsManager::load()
{
    return QJsonObject();
}

void ScriptsManager::save(const QJsonObject &obj) {}
