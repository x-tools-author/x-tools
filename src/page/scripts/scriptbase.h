/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class ScriptBase;
}

class ScriptRunner;
class ScriptBase : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptBase(QWidget *parent = nullptr);
    ~ScriptBase() override;

protected:
    virtual ScriptRunner *newRunner();
    virtual QString helpUrl() const;

private:
    void onRunButtonClicked(bool checked);
    void onNewButtonClicked();
    void onOpenButtonClicked();
    void onRefreshButtonClicked();
    void onHelpButtonClicked();

    void startRunner();
    void stopRunner();

private:
    Ui::ScriptBase *ui = nullptr;
    ScriptRunner *m_runner = nullptr;
};