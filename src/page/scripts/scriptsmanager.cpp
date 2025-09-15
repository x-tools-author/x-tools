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

    m_lua->loadScripts();
    m_js->loadScripts();

    connect(m_lua, &ScriptLua::invokeWrite, this, &ScriptsManager::invokeWrite);
    connect(m_js, &ScriptJs::invokeWrite, this, &ScriptsManager::invokeWrite);
}

ScriptsManager::~ScriptsManager()
{
    delete ui;
}

void ScriptsManager::load(const QJsonObject &obj)
{
    ScriptsManagerParameterKeys keys;
    m_lua->load(obj.value(keys.lua).toObject(QJsonObject()));
    m_js->load(obj.value(keys.js).toObject(QJsonObject()));
}

QJsonObject ScriptsManager::save()
{
    ScriptsManagerParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.lua, m_lua->save());
    obj.insert(keys.js, m_js->save());
    return obj;
}

void ScriptsManager::onBytesRead(const QByteArray &data)
{
    m_lua->onBytesRead(data);
    m_js->onBytesRead(data);
}

void ScriptsManager::aboutToClose()
{
    m_lua->aboutToClose();
    m_js->aboutToClose();
}