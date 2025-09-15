/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "scriptrunner.h"

class ScriptRunnerLua : public ScriptRunner
{
    Q_OBJECT
public:
    explicit ScriptRunnerLua(QObject *parent = nullptr);
    ~ScriptRunnerLua();

    void onBytesRead(const QByteArray &data) override;

protected:
    void run() override;

private:
    static int luaPrint(lua_State *L);
    static int luaWrite(lua_State *L);
    static int luaSleep(lua_State *L);
    static int luaIsInterruptionRequested(lua_State *L);

private:
    lua_State *m_lua = nullptr;
};
