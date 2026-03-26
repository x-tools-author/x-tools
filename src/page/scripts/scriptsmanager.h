/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/common/panel.h"

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
class ScriptsManager : public Panel
{
    Q_OBJECT
public:
    explicit ScriptsManager(QWidget *parent = nullptr);
    ~ScriptsManager();

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void onBytesRead(const QByteArray &data, const QString &flag) override;

    void aboutToClose();

private:
    Ui::ScriptsManager *ui = nullptr;
    QList<ScriptBase *> m_scripts;
};
