/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luascript.h"
#include "luascriptui.h"

#include <lua.hpp>

#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QThread>

#include "luascriptthread.h"

LuaScript::LuaScript(QObject *parent)
    : BaseNode(parent)
{
    m_handleInDataInThread.store(true);
    QThread *thread = new LuaScriptThread(this);
    thread->start();
}

LuaScript::~LuaScript() {}

QString LuaScript::caption() const
{
    return tr("Lua Script");
}

QString LuaScript::name() const
{
    return QString("LuaScript");
}

unsigned int LuaScript::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *LuaScript::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new LuaScriptUi(this);
    }

    return m_embeddedWidget;
}

bool LuaScript::resizable() const
{
    return true;
}
