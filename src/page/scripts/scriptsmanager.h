/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QObject>
#include <QWidget>

namespace Ui {
class ScriptsManager;
}

class ScriptLua;
class ScriptJs;
class ScriptsManager : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptsManager(QWidget *parent = nullptr);
    ~ScriptsManager();

    QJsonObject load();
    void save(const QJsonObject &obj);

    void onBytesRead(const QByteArray &data);

signals:
    void invokeWrite(const QByteArray &data);

private:
    Ui::ScriptsManager *ui = nullptr;
    ScriptLua *m_lua = nullptr;
    ScriptJs *m_js = nullptr;
};
