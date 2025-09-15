/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsEngine>
#include <QJsValue>

#include "scriptrunner.h"

class ScriptRunnerJs : public ScriptRunner
{
    Q_OBJECT
public:
    explicit ScriptRunnerJs(QObject *parent = nullptr);
    ~ScriptRunnerJs();

    void onBytesRead(const QByteArray &data) override;

    Q_INVOKABLE void xWriteBytes(const QJSValue &value);
    Q_INVOKABLE bool xIsInterruptionRequested();
    Q_INVOKABLE void xSleep(int ms);
    Q_INVOKABLE void xPrint(const QString &text);

protected:
    void run() override;

private:
    QJSEngine *m_engine{nullptr};
};
