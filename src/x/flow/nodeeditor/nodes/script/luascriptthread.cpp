/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luascript.h"
#include "luascriptui.h"

#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QThread>

#include "luascriptthread.h"

LuaScriptThread::LuaScriptThread(LuaScript *lua)
    : QThread(lua)
    , m_lua(lua)
{}

LuaScriptThread::~LuaScriptThread()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

void LuaScriptThread::run()
{
#if 0
    typedef std::shared_ptr<QtNodes::NodeData> Data;
    typedef QtNodes::PortIndex Index;
    QObject *obj = new QObject();

    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);
    connect(m_lua,
            &LuaScript::input2thread,
            obj,
            [=](std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex) {
                this->onInput2Thread(lua, nodeData, portIndex);
            });

    exec();
    lua_pop(lua, 0);
    obj->deleteLater();
    obj = nullptr;
#endif
}

void LuaScriptThread::onInput2Thread(lua_State *lua,
                                     std::shared_ptr<QtNodes::NodeData> nodeData,
                                     QtNodes::PortIndex const)
{
#if 0
    auto baseDataBase = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!baseDataBase) {
        qWarning("The input data is not a BaseNodeData.");
        return;
    }

    QByteArray bytes = baseDataBase->bytes();
    if (bytes.isEmpty()) {
        qWarning("The input data is empty.");
        return;
    }

    LuaScriptParametersKeys keys{};
    QJsonObject parameters = m_lua->save();
    const QString script = parameters[keys.script].toString();
    if (script.isEmpty()) {
        qWarning("The lua script is empty.");
        return;
    }

    if (luaL_dostring(lua, script.toUtf8().data()) != LUA_OK) {
        const char *err = lua_tostring(lua, -1);
        qWarning("Failed to load the lua script: %s", err);
        return;
    }

    lua_getglobal(lua, "process");
    if (!lua_isfunction(lua, -1)) {
        qWarning("The lua script does not have the function 'process'.");
        return;
    }

    lua_pushstring(lua, bytes.toStdString().c_str());
    if (lua_pcall(lua, 1, 1, 0) != LUA_OK) {
        const char *err = lua_tostring(lua, -1);
        qWarning("Failed to call the lua script: %s", err);
        return;
    }

    const char *result = lua_tostring(lua, -1);
    QByteArray outBytes(result);
    lua_pop(lua, 1);

    if (outBytes.isEmpty()) {
        qWarning("The lua script output is empty.");
        return;
    }

    m_lua->setOutData(std::make_shared<BaseNodeData>(outBytes));
    emit m_lua->dataUpdated(0);
#endif
}