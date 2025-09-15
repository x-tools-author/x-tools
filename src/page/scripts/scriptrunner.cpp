/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptrunner.h"

ScriptRunner::ScriptRunner(QObject *parent)
    : QThread(parent)
{}

ScriptRunner::~ScriptRunner() {}

void ScriptRunner::execute(const QString &scriptFile)
{
    m_scriptFile = scriptFile;
    start();
}

void ScriptRunner::onBytesRead(const QByteArray &data)
{
    Q_UNUSED(data);
}