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
#include <QWidget>

namespace Ui {
class ScriptBase;
}

struct ScriptBaseParameterKeys
{
    const QString scriptIndex = QStringLiteral("scriptIndex");
};

class ScriptRunner;
class ScriptBase : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptBase(QWidget *parent = nullptr);
    ~ScriptBase() override;

    void loadScripts();
    void onBytesRead(const QByteArray &data);
    void aboutToClose();

    virtual void load(const QJsonObject &obj);
    virtual QJsonObject save();

signals:
    void invokeWrite(const QByteArray &data);

protected:
    virtual ScriptRunner *newRunner() = 0;
    virtual QString helpUrl() const = 0;
    virtual QString scriptSuffix() const = 0;
    virtual QString scriptDir() const = 0;

    virtual QStringList ignoredFiles() const;
    void loadScriptsApp();
    virtual void loadScriptsUser();

private:
    void onScriptComboBoxCurrentIndexChanged();
    void onRunButtonClicked(bool checked);
    void onNewButtonClicked();
    void onOpenButtonClicked();
    void onRefreshButtonClicked();
    void onHelpButtonClicked();

    void startRunner();
    void stopRunner();
    void updateUiEnabled(bool running);
    void onRunnerStarted();
    void onRunnerFinished();
    void onScriptTextChanged();

private:
    Ui::ScriptBase *ui = nullptr;
    ScriptRunner *m_runner = nullptr;
};