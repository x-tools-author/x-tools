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

    return m_result;
}

QString LuaRunner::error() const
{
    return m_error;
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
    m_error.clear();
    m_result.clear();

    if (luaL_dostring(m_lua, script.toUtf8().data()) != LUA_OK) {
        const char *err = lua_tostring(m_lua, -1);
        m_error = QString("Failed to load the lua script: %1").arg(err);
        lua_pop(m_lua, 1);
        emit executed();
        return;
    }

    lua_getglobal(m_lua, "data_handler");
    if (!lua_isfunction(m_lua, -1)) {
        m_error = "The lua script does not have the function 'data_handler'.";
        lua_pop(m_lua, 1);
        emit executed();
        return;
    }

    // 创建 Lua 表（数组）来存储字节数据
    lua_newtable(m_lua);
    for (int i = 0; i < data.size(); ++i) {
        lua_pushinteger(m_lua, static_cast<unsigned char>(data[i])); // 将字节值压入栈
        lua_rawseti(m_lua, -2, i + 1); // 设置数组索引（Lua 数组从1开始）
    }
    lua_pushnumber(m_lua, data.size());

    // 2个参数，2个返回值
    if (lua_pcall(m_lua, 2, 2, 0) != LUA_OK) {
        const char *err = lua_tostring(m_lua, -1);
        m_error = QString("Failed to call the lua script: %1").arg(err);
        lua_pop(m_lua, 1);
        emit executed();
        return;
    }

    // 检查返回值数量 - 现在栈顶应该有2个值
    if (lua_gettop(m_lua) < 2) {
        m_error = "Lua function 'data_handler' must return 2 values (data, length).";
        lua_settop(m_lua, 0); // 清空栈
        emit executed();
        return;
    }

    // 获取第二个返回值（长度） - 在栈顶
    if (!lua_isnumber(m_lua, -1)) {
        m_error = "Second return value from 'data_handler' must be a number (length).";
        lua_pop(m_lua, 2); // 清除两个返回值
        emit executed();
        return;
    }
    int resultLength = static_cast<int>(lua_tonumber(m_lua, -1));
    lua_pop(m_lua, 1); // 移除长度值

    // 获取第一个返回值（数据） - 现在在栈顶
    if (!lua_istable(m_lua, -1)) {
        m_error = "First return value from 'data_handler' must be a table (array).";
        lua_pop(m_lua, 1); // 清除数据值
        emit executed();
        return;
    }

    // 从 Lua 表中读取字节数据
    m_result.clear();
    m_result.reserve(resultLength); // 预分配空间以提高性能

    for (int i = 1; i <= resultLength; ++i) { // Lua 数组从1开始
        lua_rawgeti(m_lua, -1, i);            // 获取数组元素
        if (lua_isnumber(m_lua, -1)) {
            int byteValue = static_cast<int>(lua_tonumber(m_lua, -1));
            m_result.append(static_cast<char>(byteValue & 0xFF)); // 确保是有效的字节值
        } else {
            qWarning() << "Array element at index" << i << "is not a number, skipping";
        }
        lua_pop(m_lua, 1); // 移除元素值
    }

    lua_pop(m_lua, 1); // 清除数据值
    emit executed();
}