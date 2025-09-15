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

ScriptJs::ScriptJs(QWidget *parent)
    : ScriptBase(parent)
{}

ScriptJs::~ScriptJs() {}

ScriptRunner *ScriptJs::newRunner()
{
    return new ScriptRunnerJs(this);
}

QString ScriptJs::helpUrl() const
{
    return QStringLiteral("https://developer.mozilla.org/en-US/docs/Web/JavaScript");
}

QString ScriptJs::scriptSuffix() const
{
    return QStringLiteral("js");
}

QString ScriptJs::scriptDir() const
{
    return QStringLiteral("js");
}
