/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <QThread>

class LuaRunner : public QThread
{
    Q_OBJECT
public:
    enum ParameterType { ParameterTypeTable, ParameterTypeString };

public:
    explicit LuaRunner(QObject *parent = nullptr);
    ~LuaRunner() override;

    QByteArray execute(const QString &script, const QByteArray &data, int type);
    QString error() const;

protected:
    void run() override;

private:
    QByteArray m_result;
    QString m_error;
    lua_State *m_lua{nullptr};

private:
    Q_SIGNAL void invokeExecute(const QString &functionName, const QByteArray &data, int type);
    Q_SIGNAL void executed();

    void executeInThread(const QString &script, const QByteArray &data, int type);
    void executeInThreadTable(const QString &script, const QByteArray &data);
    void executeInThreadString(const QString &script, const QByteArray &data);
};