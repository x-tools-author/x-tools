/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "scriptbase.h"

class ScriptLua : public ScriptBase
{
    Q_OBJECT
public:
    explicit ScriptLua(QWidget *parent = nullptr);
    ~ScriptLua();

protected:
    ScriptRunner *newRunner() override;
    QString helpUrl() const override;
    QString scriptSuffix() const override;
    QString scriptDir() const override;

    QStringList ignoredFiles() const override;
};