/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptlua.h"

#include "common/syntaxhighlighterlua.h"
#include "scriptrunnerlua.h"

ScriptLua::ScriptLua(QWidget *parent)
    : ScriptBase(parent)
{
    auto ret = new SyntaxHighlighterLua(scriptDocument());
    Q_UNUSED(ret);
}

ScriptLua::~ScriptLua() {}

ScriptRunner *ScriptLua::newRunner()
{
    return new ScriptRunnerLua(this);
}

QString ScriptLua::helpUrl() const
{
    return QStringLiteral("https://x-tools-author.github.io/x-tools/docs/06.scripts/1.lua/");
}

QString ScriptLua::scriptSuffix() const
{
    return QStringLiteral("lua");
}

QString ScriptLua::scriptDir() const
{
    return QStringLiteral("lua");
}

QStringList ScriptLua::ignoredFiles() const
{
    return QStringList() << "check_sum.lua"
                         << "default_string.lua" << "default.lua";
}