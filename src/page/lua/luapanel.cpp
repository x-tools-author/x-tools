/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luapanel.h"
#include "ui_luapanel.h"

#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QTimer>

#include "common/xapp.h"
#include "common/xtools.h"
#include "luarunner.h"
#include "utilities/compatibility.h"
#include "utilities/highlighter/syntaxhighlighterlua.h"
#include "utilities/iconengine.h"

namespace Ui {
class LuaPanel;
}

struct LuaPanelDataKeys
{
    const QString type = "type";
    const QString script = "script";
    const QString testFormat = "testFormat";
    const QString resultFormat = "resultFormat";
    const QString bypass = "bypass";
    const QString testData = "testData";
};

static QString luaScriptsRootDir()
{
    QString rootPath = xAPP->settingsPath();
    rootPath.append("/lua_scripts");
    QDir dir(rootPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return rootPath;
}

class LuaPanelPrivate : public QObject
{
public:
    LuaPanelPrivate(LuaPanel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~LuaPanelPrivate() {}

public:
    void onTestFormatChanged()
    {
        int format = ui->comboBoxTestFormat->currentData().toInt();
        xSetupTextFormatValidator(ui->lineEditTestData, format);
        ui->lineEditTestData->setText(xBytes2string(m_testData, format));
    }
    void onResultFormatChanged()
    {
        int format = ui->comboBoxResultFormat->currentData().toInt();
        xSetupTextFormatValidator(ui->lineEditResultData, format);
        ui->lineEditResultData->setText(xBytes2string(m_resultData, format));
    }
    void onTestButtonClicked()
    {
        ui->labelInfo->clear();

        int type = ui->radioButtonString->isChecked() ? int(LuaRunner::ParameterTypeString)
                                                      : int(LuaRunner::ParameterTypeTable);
        QString script = ui->plainTextEditScript->toPlainText();
        int testFormat = ui->comboBoxTestFormat->currentData().toInt();
        QByteArray data = xString2bytes(ui->lineEditTestData->text(), testFormat);
        int resultFormat = ui->comboBoxResultFormat->currentData().toInt();

        m_resultData = m_luaRunner->execute(script, data, type);
        QString str = xBytes2string(m_resultData, resultFormat);
        ui->lineEditResultData->setText(str);

        if (m_resultData.isEmpty()) {
            QString errStr = m_luaRunner->error();
            ui->labelInfo->setText(errStr);
        }
    }
    void onSaveButtonClicked()
    {
        QString scriptPath = ui->comboBoxScripts->currentData().toString();
        if (scriptPath.isEmpty()) {
            QString msg = tr("No script selected to save.");
            showInfo(msg);
            return;
        }

        QFile file(scriptPath);
        if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
            QString msg = tr("Cannot open file for writing: %1. Error: %2")
                              .arg(scriptPath, file.errorString());
            qWarning().noquote() << msg;
            showInfo(msg);
            return;
        }

        QByteArray content = ui->plainTextEditScript->toPlainText().toUtf8();
        qint64 bytesWritten = file.write(content);
        file.close();

        if (bytesWritten == -1) {
            QString msg = tr("Save file: %1. Error: %2").arg(scriptPath, file.errorString());
            qWarning().noquote() << msg;
            showInfo(msg);
        }
    }
    void onNewButtonClicked()
    {
        QString fileName = QInputDialog::getText(q, tr("New Lua Script"), tr("Enter script name:"));
        if (fileName.isEmpty()) {
            return;
        }

        if (!fileName.endsWith(".lua", Qt::CaseInsensitive)) {
            fileName += ".lua";
        }

        QString rootPath = luaScriptsRootDir();
        QString newFilePath = rootPath + "/" + fileName;
        if (QFile::exists(newFilePath)) {
            QString msg = tr("A script with the name '%1' already exists.").arg(fileName);
            showInfo(msg);
            return;
        }

        QFile newFile(newFilePath);
        if (!newFile.open(QFile::WriteOnly | QFile::Text)) {
            QString msg = tr("Cannot create file: %1. Error: %2")
                              .arg(newFilePath, newFile.errorString());
            qWarning().noquote() << msg;
            showInfo(msg);
            return;
        }

        newFile.close();
        loadLuaScripts();

        int index = ui->comboBoxScripts->findData(newFilePath);
        if (index != -1) {
            ui->comboBoxScripts->setCurrentIndex(index);
            onScriptChanged();
        }
    }
    void onSaveAsButtonClicked()
    {
        QString scriptPath = ui->comboBoxScripts->currentData().toString();
        if (scriptPath.isEmpty()) {
            QString msg = tr("No script selected to save.");
            showInfo(msg);
            return;
        }

        QString newFilePath = QInputDialog::getText(q,
                                                    tr("Save Lua Script As"),
                                                    tr("Enter new script name:"));
        if (newFilePath.isEmpty()) {
            return;
        }

        if (!newFilePath.endsWith(".lua", Qt::CaseInsensitive)) {
            newFilePath += ".lua";
        }

        QString rootPath = luaScriptsRootDir();
        newFilePath = rootPath + "/" + QFileInfo(newFilePath).fileName();
        QFile file(scriptPath);
        if (!file.copy(newFilePath)) {
            QString msg = tr("Cannot save file as: %1. Error: %2")
                              .arg(newFilePath, file.errorString());
            qWarning().noquote() << msg;
            showInfo(msg);
            return;
        }

        loadLuaScripts();

        int index = ui->comboBoxScripts->findData(newFilePath);
        if (index != -1) {
            ui->comboBoxScripts->setCurrentIndex(index);
            onScriptChanged();
        }
    }
    void onDirectoryButtonClicked()
    {
        QString luaScriptsDir = luaScriptsRootDir();
        QDesktopServices::openUrl(QUrl::fromLocalFile(luaScriptsDir));
    }
    void onScriptChanged()
    {
        QString scriptPath = ui->comboBoxScripts->currentData().toString();
        if (scriptPath.isEmpty()) {
            return;
        }

        QFile file(scriptPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->labelInfo->setText(tr("Failed to open the script: %1").arg(scriptPath));
            return;
        }

        QByteArray content = file.readAll();
        file.close();
        ui->plainTextEditScript->setPlainText(QString::fromUtf8(content));
    }

public:
    void loadLuaScripts()
    {
        ui->comboBoxScripts->clear();
        QString rootPath = luaScriptsRootDir();
        QDir dir(rootPath);
        QStringList filters;
        filters << "*.lua";
        QDir::Filters entryFilters = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot;
        QFileInfoList fileInfoList = dir.entryInfoList(filters, entryFilters);
        for (const QFileInfo &fileInfo : fileInfoList) {
            QString scriptName = fileInfo.baseName();
            QString scriptPath = fileInfo.absoluteFilePath();
            ui->comboBoxScripts->addItem(scriptName, scriptPath);
        }

        static bool extracted = false;
        if (fileInfoList.isEmpty() && !extracted) {
            QFile stringFile(":/res/scripts/lua/default_string.lua");
            if (!stringFile.copy(rootPath + "/default_string.lua")) {
                qWarning() << "Failed to extract default_string.lua:" << stringFile.errorString();
            }

            // Add write permission to the extracted file, otherwise it cannot be modified and saved.
            QFile::setPermissions(rootPath + "/default_string.lua",
                                  QFile::permissions(rootPath + "/default_string.lua")
                                      | QFile::WriteOwner);

            QFile tableFile(":/res/scripts/lua/default.lua");
            if (!tableFile.copy(rootPath + "/default.lua")) {
                qWarning() << "Failed to extract default.lua:" << tableFile.errorString();
            }

            extracted = true;
            loadLuaScripts();
        }

        if (!fileInfoList.isEmpty()) {
            int index = ui->comboBoxScripts->findData(rootPath + "/default_string.lua");
            if (index != -1) {
                ui->comboBoxScripts->setCurrentIndex(index);
            }
            onScriptChanged();
        }
    }
    void showInfo(const QString &info)
    {
        ui->labelInfo->setText(info);
        QTimer::singleShot(5000, [this]() { ui->labelInfo->clear(); });
    }

public:
    Ui::LuaPanel *ui;
    LuaRunner *m_luaRunner;
    const QByteArray m_testData{"Hello, Lua!"};
    QByteArray m_resultData;
    xTools::SyntaxHighlighterLua *m_syntaxHighlighterTable;
    xTools::SyntaxHighlighterLua *m_syntaxHighlighterString;

private:
    LuaPanel *q;
};

LuaPanel::LuaPanel(QWidget *parent)
    : Panel(parent)
{
    d = new LuaPanelPrivate(this);
    d->ui = new Ui::LuaPanel;
    d->ui->setupUi(this);
    d->ui->checkBoxBypass->setChecked(true);
    d->ui->toolButtonHelp->setIcon(xIcon(":/res/icons/help.svg"));
    d->ui->toolButtonHelp->hide();
    d->m_luaRunner = new LuaRunner(this);
    d->m_luaRunner->start();

    // clang-format off
    connect(d->ui->pushButtonTest, &QPushButton::clicked, d, &LuaPanelPrivate::onTestButtonClicked);
    connect(d->ui->comboBoxTestFormat, xComboBoxActivated, d, &LuaPanelPrivate::onTestFormatChanged);
    connect(d->ui->comboBoxResultFormat, xComboBoxActivated, d, &LuaPanelPrivate::onResultFormatChanged);
    connect(d->ui->toolButtonSave, &QToolButton::clicked, d, &LuaPanelPrivate::onSaveButtonClicked);
    connect(d->ui->toolButtonNew, &QToolButton::clicked, d, &LuaPanelPrivate::onNewButtonClicked);
    connect(d->ui->toolButtonSaveAs, &QToolButton::clicked, d, &LuaPanelPrivate::onSaveAsButtonClicked);
    connect(d->ui->toolButtonDirectory, &QToolButton::clicked, d, &LuaPanelPrivate::onDirectoryButtonClicked);
    connect(d->ui->comboBoxScripts, xComboBoxActivated, d, &LuaPanelPrivate::onScriptChanged);
    // clang-format on

    xSetupTextFormat(d->ui->comboBoxTestFormat);
    xSetupTextFormat(d->ui->comboBoxResultFormat);
    d->onTestFormatChanged();
    d->onResultFormatChanged();
    d->onScriptChanged();
    d->loadLuaScripts();

    QTextDocument *doc = d->ui->plainTextEditScript->document();
    d->m_syntaxHighlighterTable = new xTools::SyntaxHighlighterLua(doc);
}

LuaPanel::~LuaPanel()
{
    delete d->ui;
}

QVariantMap LuaPanel::save() const
{
    QVariantMap map = Panel::save();
    LuaPanelDataKeys keys;

    const int strtingType = int(LuaRunner::ParameterTypeString);
    const int tableType = int(LuaRunner::ParameterTypeTable);
    int type = d->ui->radioButtonString->isChecked() ? strtingType : tableType;
    map[keys.type] = type;
    map[keys.script] = d->ui->comboBoxScripts->currentData().toString();
    map[keys.testFormat] = d->ui->comboBoxTestFormat->currentData().toInt();
    map[keys.resultFormat] = d->ui->comboBoxResultFormat->currentData().toInt();
    map[keys.bypass] = d->ui->checkBoxBypass->isChecked();
    map[keys.testData] = d->ui->lineEditTestData->text();
    return map;
}

void LuaPanel::load(const QVariantMap &parameters)
{
    Panel::load(parameters);

    int defaultTestFormat = static_cast<int>(TextFormat::Hex);
    QString defaultTestData = xBytes2string(d->m_testData, defaultTestFormat);
    LuaPanelDataKeys keys;

    int type = parameters.value(keys.type, int(LuaRunner::ParameterTypeString)).toInt();
    if (type == int(LuaRunner::ParameterTypeTable)) {
        d->ui->radioButtonArray->setChecked(true);
    } else {
        d->ui->radioButtonString->setChecked(true);
    }

    QString scriptPath = parameters.value(keys.script).toString();
    int index = d->ui->comboBoxScripts->findData(scriptPath);
    if (index != -1) {
        d->ui->comboBoxScripts->setCurrentIndex(index);
        d->onScriptChanged();
    }

    int testFormat = parameters.value(keys.testFormat, defaultTestFormat).toInt();
    index = d->ui->comboBoxTestFormat->findData(testFormat);
    d->ui->comboBoxTestFormat->setCurrentIndex(index);
    xSetupTextFormatValidator(d->ui->lineEditTestData, testFormat);

    int resultFormat = parameters.value(keys.resultFormat, testFormat).toInt();
    index = d->ui->comboBoxResultFormat->findData(resultFormat);
    d->ui->comboBoxResultFormat->setCurrentIndex(index);
    xSetupTextFormatValidator(d->ui->lineEditResultData, resultFormat);

#if 0
    bool bypass = parameters.value(keys.bypass, true).toBool();
    d->ui->checkBoxBypass->setChecked(bypass);
#endif

    QString testData = parameters.value(keys.testData, defaultTestData).toString();
    d->ui->lineEditTestData->setText(testData);
}

QByteArray LuaPanel::handleData(const QByteArray &data) const
{
    if (d->ui->checkBoxBypass->isChecked()) {
        return data;
    } else {
        int type = d->ui->radioButtonString->isChecked() ? int(LuaRunner::ParameterTypeString)
                                                         : int(LuaRunner::ParameterTypeTable);
        return d->m_luaRunner->execute(d->ui->plainTextEditScript->toPlainText(), data, type);
    }
}

bool LuaPanel::isBypassed() const
{
    return d->ui->checkBoxBypass->isChecked();
}
