/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptjs.h"

#include "scriptrunnerjs.h"
#include "utilities/highlighter/syntaxhighlighterjs.h"

ScriptJs::ScriptJs(QWidget *parent)
    : ScriptBase(parent)
{
    auto ret = new xTools::SyntaxHighlighterJs(scriptDocument());
    Q_UNUSED(ret);
}

ScriptJs::~ScriptJs() {}

ScriptRunner *ScriptJs::newRunner()
{
    return new ScriptRunnerJs(this);
}

QString ScriptJs::helpUrl() const
{
    return QStringLiteral("https://x-tools-author.github.io/x-tools/docs/06.scripts/2.javascript/");
}

QString ScriptJs::scriptSuffix() const
{
    return QStringLiteral("js");
}

QString ScriptJs::scriptDir() const
{
    return QStringLiteral("js");
}
