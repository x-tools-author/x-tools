/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptrunnerlua.h"

#include <QDebug>
#include <QEventLoop>
#include <QTimer>

ScriptRunnerLua::ScriptRunnerLua(QObject *parent)
    : ScriptRunner(parent)
{}

ScriptRunnerLua::~ScriptRunnerLua() {}

void ScriptRunnerLua::onBytesRead(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    if (m_lua == nullptr) {
        return;
    }

    lua_getglobal(m_lua, "x_read");
    if (lua_isfunction(m_lua, -1)) {
        lua_pushlstring(m_lua, data.constData(), data.size());
        if (lua_pcall(m_lua, 1, 0, 0) != LUA_OK) {
            const char *errorMsg = lua_tostring(m_lua, -1);
            emit logOutput(QString("Lua Error: %1").arg(QString(errorMsg)));
            lua_pop(m_lua, 1); // Remove error message from the stack
        }
    } else {
        lua_pop(m_lua, 1); // Remove non-function value from the stack
    }
}

void ScriptRunnerLua::run()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);
    lua_pushlightuserdata(m_lua, this);
    lua_setglobal(m_lua, "LUA_RUNNER_INSTANCE");
    lua_register(m_lua, "print", &ScriptRunnerLua::luaPrint);
    lua_register(m_lua, "x_print", &ScriptRunnerLua::luaPrint);
    lua_register(m_lua, "x_write", &ScriptRunnerLua::luaWrite);
    lua_register(m_lua, "x_sleep", &ScriptRunnerLua::luaSleep);
    lua_register(m_lua, "x_is_interruption_requested", &ScriptRunnerLua::luaIsInterruptionRequested);

    int ret = luaL_dofile(m_lua, m_scriptFile.toUtf8().constData());
    if (ret != LUA_OK) {
        const char *errorMsg = lua_tostring(m_lua, -1);
        emit logOutput(QString("Lua Error: %1").arg(QString(errorMsg)));
        lua_pop(m_lua, 1); // Remove error message from the stack
    }

    exec();
    lua_close(m_lua);
    m_lua = nullptr;
}

int ScriptRunnerLua::luaPrint(lua_State *L)
{
    int n = lua_gettop(L);
    QString output;

    for (int i = 1; i <= n; i++) {
        if (lua_isstring(L, i)) {
            output += lua_tostring(L, i);
        } else if (lua_isnumber(L, i)) {
            output += QString("%1").arg(lua_tonumber(L, i));
        } else if (lua_isboolean(L, i)) {
            output += lua_toboolean(L, i) ? QString("true") : QString("false");
        } else if (lua_isnil(L, i)) {
            output += QString("nil");
        } else {
            output += QString("(%1)").arg(luaL_typename(L, i));
        }

        if (i < n) {
            output += "\t";
        }
    }

    lua_getglobal(L, "LUA_RUNNER_INSTANCE");
    ScriptRunnerLua *runner = static_cast<ScriptRunnerLua *>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (runner) {
        emit runner->logOutput(output);
    } else {
        qInfo() << QString("[Lua]") << output;
    }

    return 0;
}

int ScriptRunnerLua::luaWrite(lua_State *L)
{
    const char *data = luaL_checkstring(L, 1);
    if (data) {
        lua_getglobal(L, "LUA_RUNNER_INSTANCE");
        ScriptRunnerLua *runner = static_cast<ScriptRunnerLua *>(lua_touserdata(L, -1));
        lua_pop(L, 1);

        if (runner) {
            QString strData = QString::fromUtf8(data);
            emit runner->invokeWrite(strData.toUtf8());
        } else {
            qInfo() << QString("[Lua]") << data;
        }
    }

    return 0;
}

int ScriptRunnerLua::luaSleep(lua_State *L)
{
    int ms = (int) luaL_checkinteger(L, 1);
    if (ms > 0) {
        QEventLoop loop;
        QTimer::singleShot(ms, &loop, &QEventLoop::quit);
        loop.exec();
    }
    return 0;
}

int ScriptRunnerLua::luaIsInterruptionRequested(lua_State *L)
{
    lua_getglobal(L, "LUA_RUNNER_INSTANCE");
    ScriptRunnerLua *runner = static_cast<ScriptRunnerLua *>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (runner) {
        return runner->isInterruptionRequested() ? 1 : 0;
    } else {
        return 1;
    }
}