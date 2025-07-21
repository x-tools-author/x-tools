/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luarunner.h"

#include <QDebug>
#include <QEventLoop>

LuaRunner::LuaRunner(QObject *parent)
    : QThread(parent)
{}

LuaRunner::~LuaRunner()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

QByteArray LuaRunner::execute(const QString &script, const QByteArray &data)
{
    emit invokeExecute(script, data);

    QEventLoop loop;
    connect(this, &LuaRunner::executed, &loop, &QEventLoop::quit);
    loop.exec();

    if (!m_error.isEmpty()) {
        qWarning() << "Lua execution error:" << m_error;
        return QByteArray();
    }

    return m_result;
}

void LuaRunner::run()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);

    QObject *obj = new QObject();
    connect(this,
            &LuaRunner::invokeExecute,
            obj,
            [this](const QString &functionName, const QByteArray &data) {
                executeInThread(functionName, data);
            });

    exec();
    lua_close(m_lua);
    obj->deleteLater();
    obj = Q_NULLPTR;
}

void LuaRunner::executeInThread(const QString &script, const QByteArray &data)
{
    // The function name is 'process' and it should be defined in the script string.
    // the 'process' function will be called with the data as an argument.
    lua_getglobal(m_lua, script.toUtf8().constData());
    lua_pushlstring(m_lua, data.constData(), data.size());
    lua_pcall(m_lua, 1, 1, 0);
    m_result = lua_tostring(m_lua, -1);
    lua_pop(m_lua, 1);
}
