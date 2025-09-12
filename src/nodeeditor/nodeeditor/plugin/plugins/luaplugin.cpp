/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luaplugin.h"

#include <lua.hpp>

LuaPlugin::LuaPlugin(const QString &libFile, QObject *parent)
    : AbstractPlugin(libFile, parent)
{
    // Initialize Lua state and other resources here
}

LuaPlugin::~LuaPlugin()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

bool LuaPlugin::testLib(const QString &libFile)
{
    LuaPluginApiV1 apis;
    lua_State *lua = luaL_newstate();
    if (!lua) {
        qWarning() << "Failed to create Lua state for plugin:" << libFile;
        return false;
    }

    luaL_openlibs(lua);
    luaL_dofile(lua, libFile.toStdString().c_str());
    int apiVersion = lua_getglobal(lua, apis.apiVersion.toStdString().c_str());
    if (apiVersion != LUA_TFUNCTION) {
        qWarning("The file(%s) is missing function: %s",
                 libFile.toStdString().c_str(),
                 apis.apiVersion.toStdString().c_str());
        lua_close(lua);
        return false;
    }

    // int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc);
    if (lua_pcall(lua, 0, 1, 0) != LUA_OK) {
        const char *err = lua_tostring(lua, -1);
        qWarning("Failed to call function : %s, because: %s",
                 apis.apiVersion.toStdString().c_str(),
                 err);
        lua_pop(lua, 1);
        lua_close(lua);
        return false;
    }

    if (lua_isstring(lua, -1) != 1) {
        qWarning("The result of the function %s is not a string",
                 apis.apiVersion.toStdString().c_str());
        lua_pop(lua, 1);
        lua_close(lua);
        return false;
    }

    const char *version = lua_tostring(lua, -1);
    if (QByteArray(version) != QByteArray("1.0")) {
        qWarning("The version(%s) of the plugin is not 1.0", version);
        lua_pop(lua, 1);
        lua_close(lua);
        return false;
    }

    int caption = lua_getglobal(lua, apis.pluginCaption.toStdString().c_str());
    int name = lua_getglobal(lua, apis.pluginName.toStdString().c_str());
    int inPorts = lua_getglobal(lua, apis.pluginInPorts.toStdString().c_str());
    int outPorts = lua_getglobal(lua, apis.pluginOutPorts.toStdString().c_str());
    int handle = lua_getglobal(lua, apis.pluginHandle.toStdString().c_str());
    if (caption == LUA_TFUNCTION && name == LUA_TFUNCTION && inPorts == LUA_TFUNCTION
        && outPorts == LUA_TFUNCTION && handle == LUA_TFUNCTION) {
        lua_pop(lua, 5);
        return true;
    }

    lua_close(lua);
    return false;
}

QString LuaPlugin::caption() const
{
    LuaPluginApiV1 apis;
    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);
    luaL_dofile(lua, m_libFile.toStdString().c_str());

    int ret = lua_getglobal(lua, apis.pluginCaption.toStdString().c_str());
    if (ret == LUA_TFUNCTION) {
        lua_call(lua, 0, 1); // Call the function to get the caption
        if (lua_isstring(lua, -1)) {
            const char *caption = lua_tostring(lua, -1);
            lua_pop(lua, 1); // Pop the caption string from the stack
            lua_close(lua);
            return QString::fromLocal8Bit(caption);
        }
    }

    qWarning() << "Invalid Lua plugin:" << m_libFile << "missing function:" << apis.pluginCaption;
    lua_close(lua);
    return QString(__FUNCTION__);
}

QString LuaPlugin::name() const
{
    LuaPluginApiV1 apis;
    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);
    luaL_dofile(lua, m_libFile.toStdString().c_str());

    int ret = lua_getglobal(lua, apis.pluginName.toStdString().c_str());
    if (ret == LUA_TFUNCTION) {
        lua_call(lua, 0, 1); // Call the function to get the name
        if (lua_isstring(lua, -1)) {
            const char *name = lua_tostring(lua, -1);
            lua_pop(lua, 1); // Pop the name string from the stack
            lua_close(lua);
            return QString::fromLocal8Bit(name);
        }
    }

    qWarning() << "Invalid Lua plugin:" << m_libFile << "missing function:" << apis.pluginName;
    lua_close(lua);
    return QString(__FUNCTION__);
}

unsigned int LuaPlugin::inPorts() const
{
    LuaPluginApiV1 apis;
    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);
    luaL_dofile(lua, m_libFile.toStdString().c_str());

    int ret = lua_getglobal(lua, apis.pluginInPorts.toStdString().c_str());
    if (ret == LUA_TFUNCTION) {
        lua_call(lua, 0, 1); // Call the function to get the number of input ports
        if (lua_isnumber(lua, -1)) {
            unsigned int inPorts = static_cast<unsigned int>(lua_tointeger(lua, -1));
            lua_pop(lua, 1); // Pop the number from the stack
            lua_close(lua);
            return inPorts;
        }
    }

    qWarning() << "Invalid Lua plugin:" << m_libFile << "missing function:" << apis.pluginInPorts;
    lua_close(lua);
    return 0;
}

unsigned int LuaPlugin::outPorts() const
{
    LuaPluginApiV1 apis;
    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);
    luaL_dofile(lua, m_libFile.toStdString().c_str());

    int ret = lua_getglobal(lua, apis.pluginOutPorts.toStdString().c_str());
    if (ret == LUA_TFUNCTION) {
        lua_call(lua, 0, 1); // Call the function to get the number of output ports
        if (lua_isnumber(lua, -1)) {
            unsigned int outPorts = static_cast<unsigned int>(lua_tointeger(lua, -1));
            lua_pop(lua, 1); // Pop the number from the stack
            lua_close(lua);
            return outPorts;
        }
    }

    qWarning() << "Invalid Lua plugin:" << m_libFile << "missing function:" << apis.pluginOutPorts;
    lua_close(lua);
    return 0;
}

QByteArray LuaPlugin::handleData(QByteArray const &data, int const index)
{
    LuaPluginApiV1 apis;
    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);
    luaL_dofile(lua, m_libFile.toStdString().c_str());

    int ret = lua_getglobal(lua, apis.pluginHandle.toStdString().c_str());
    if (ret == LUA_TFUNCTION) {
        lua_pushlstring(lua, data.constData(), data.size());
        lua_pushinteger(lua, index);
        if (lua_pcall(lua, 2, 1, 0) == LUA_OK) {
            if (lua_isstring(lua, -1) == 1) {
                const char *outData = lua_tostring(lua, -1);
                QByteArray result = QByteArray::fromRawData(outData, strlen(outData));
                lua_pop(lua, 1);
                lua_close(lua);
                return result;
            }
        }
    }

    qWarning() << "Invalid Lua plugin:" << m_libFile << "missing function:" << apis.pluginOutPorts;
    lua_close(lua);
    return QByteArray();
}