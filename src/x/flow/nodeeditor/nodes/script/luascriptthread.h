/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <lua.h>
#include <QtNodes/Definitions>
#include <QtNodes/internal/NodeData.hpp>

#include <QThread>

class LuaScriptThread : public QThread
{
    Q_OBJECT
public:
    LuaScriptThread(LuaScript *lua);
    ~LuaScriptThread() override;

private:
    LuaScript *m_lua;

protected:
    void run() override;
    void onInput2Thread(lua_State *lua,
                        std::shared_ptr<QtNodes::NodeData> nodeData,
                        QtNodes::PortIndex const);
};
