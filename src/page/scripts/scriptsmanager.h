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

struct ScriptsManagerParameterKeys
{
    const QString tabIndex{"tabIndex"};
};

class ScriptBase;
class ScriptsManager : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptsManager(QWidget *parent = nullptr);
    ~ScriptsManager();

    void load(const QJsonObject &obj);
    QJsonObject save();

    void onBytesRead(const QByteArray &data);
    void aboutToClose();

signals:
    void invokeWrite(const QByteArray &data);

private:
    Ui::ScriptsManager *ui = nullptr;
    QList<ScriptBase *> m_scripts;
};
