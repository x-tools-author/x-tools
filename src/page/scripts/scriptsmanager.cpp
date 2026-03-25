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
    : Panel(parent)
    , ui(new Ui::ScriptsManager)
{
    ui->setupUi(this);

    ScriptBase *tmp = new ScriptLua(this);
    m_scripts.append(tmp);
    ui->tabWidget->addTab(tmp, QString("Lua"));

    tmp = new ScriptJs(this);
    m_scripts.append(tmp);
    ui->tabWidget->addTab(tmp, QString("JavaScript"));

    for (ScriptBase *script : const_cast<QList<ScriptBase *> &>(m_scripts)) {
        script->loadScripts();
        connect(script, &ScriptBase::invokeWrite, this, &ScriptsManager::invokeWrite);
    }
}

ScriptsManager::~ScriptsManager()
{
    delete ui;
}

QVariantMap ScriptsManager::save() const
{
    ScriptsManagerParameterKeys keys;
    QVariantMap obj;
    obj.insert(keys.tabIndex, ui->tabWidget->currentIndex());

    for (ScriptBase *script : const_cast<QList<ScriptBase *> &>(m_scripts)) {
        obj.insert(script->metaObject()->className(), script->save());
    }

    return obj;
}

void ScriptsManager::load(const QVariantMap &parameters)
{
    ScriptsManagerParameterKeys keys;
    int index = parameters.value(keys.tabIndex).toInt(0);
    index = index < 0 ? 0 : (index >= ui->tabWidget->count() ? 0 : index);
    ui->tabWidget->setCurrentIndex(index);

    for (ScriptBase *script : const_cast<QList<ScriptBase *> &>(m_scripts)) {
        if (parameters.contains(script->metaObject()->className())) {
            script->load(parameters.value(script->metaObject()->className()).toJsonObject());
        }
    }
}

void ScriptsManager::onBytesRead(const QByteArray &data, const QString &flag)
{
    for (ScriptBase *script : const_cast<QList<ScriptBase *> &>(m_scripts)) {
        script->onBytesRead(data);
    }
}

void ScriptsManager::aboutToClose()
{
    for (ScriptBase *script : const_cast<QList<ScriptBase *> &>(m_scripts)) {
        script->aboutToClose();
    }
}