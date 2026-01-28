/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "luapanel.h"
#include "ui_luapanel.h"

#include <QFile>

#include "common/xtools.h"
#include "luarunner.h"
#include "utilities/compatibility.h"
#include "utilities/syntaxhighlighterlua.h"

struct LuaPanelDataKeys
{
    const QString type = "type";
    const QString scriptTable = "scriptTable";
    const QString scriptString = "scriptString";
    const QString testFormat = "testFormat";
    const QString resultFormat = "resultFormat";
    const QString bypass = "bypass";
    const QString testData = "testData";
};

LuaPanel::LuaPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::LuaPanel)
    , m_menu(nullptr)
{
    ui->setupUi(this);
    ui->checkBoxBypass->setChecked(true);

    m_menu = new QMenu(this);
    m_menu->addAction(tr("Default Lua Script"), this, &LuaPanel::onDefaultLuaScriptTriggered);
    m_menu->addAction(tr("Checksum Lua Script"), this, &LuaPanel::onCheckSumLuaScriptTriggered);
    m_menu->addSeparator();
    m_menu->addAction(tr("Lua Script(String)"), this, &LuaPanel::onDefaultLuaScriptStringTriggered);
    onDefaultLuaScriptTriggered();

    m_luaRunner = new LuaRunner(this);
    m_luaRunner->start();

    connect(ui->pushButtonTest, &QPushButton::clicked, this, &LuaPanel::onTestButtonClicked);
    connect(ui->comboBoxTestFormat, xComboBoxActivated, this, &LuaPanel::onTestFormatChanged);
    connect(ui->comboBoxResultFormat, xComboBoxActivated, this, &LuaPanel::onResultFormatChanged);
    xSetupTextFormat(ui->comboBoxTestFormat);
    xSetupTextFormat(ui->comboBoxResultFormat);
    xSetNoneBorder(ui->plainTextEditScriptString);
    xSetNoneBorder(ui->plainTextEditScriptTable);
    onTestFormatChanged();
    onResultFormatChanged();

    m_syntaxHighlighterTable = new xTools::SyntaxHighlighterLua(
        ui->plainTextEditScriptTable->document());
    m_syntaxHighlighterString = new xTools::SyntaxHighlighterLua(
        ui->plainTextEditScriptString->document());
}

LuaPanel::~LuaPanel()
{
    delete ui;
}

QVariantMap LuaPanel::save() const
{
    QVariantMap map = Panel::save();
    LuaPanelDataKeys keys;

    map[keys.type] = ui->tabWidgetLua->currentIndex();
    map[keys.scriptTable] = ui->plainTextEditScriptTable->toPlainText();
    map[keys.scriptString] = ui->plainTextEditScriptString->toPlainText();
    map[keys.testFormat] = ui->comboBoxTestFormat->currentData().toInt();
    map[keys.resultFormat] = ui->comboBoxResultFormat->currentData().toInt();
    map[keys.bypass] = ui->checkBoxBypass->isChecked();
    map[keys.testData] = ui->lineEditTestData->text();
    return map;
}

void LuaPanel::load(const QVariantMap &parameters)
{
    Panel::load(parameters);

    int defaultTestFormat = static_cast<int>(TextFormat::Hex);
    QString defaultTestData = xBytes2string(m_testData, defaultTestFormat);
    LuaPanelDataKeys keys;

    int type = parameters.value(keys.type, 0).toInt();
    ui->tabWidgetLua->setCurrentIndex(type);

    ui->plainTextEditScriptTable->setPlainText(parameters[keys.scriptTable].toString());
    if (ui->plainTextEditScriptTable->toPlainText().isEmpty()) {
        onDefaultLuaScriptTriggered();
    }

    ui->plainTextEditScriptString->setPlainText(parameters[keys.scriptString].toString());
    if (ui->plainTextEditScriptString->toPlainText().isEmpty()) {
        onDefaultLuaScriptStringTriggered();
    }

    int testFormat = parameters.value(keys.testFormat, defaultTestFormat).toInt();
    int index = ui->comboBoxTestFormat->findData(testFormat);
    ui->comboBoxTestFormat->setCurrentIndex(index);
    xSetupTextFormatValidator(ui->lineEditTestData, testFormat);

    int resultFormat = parameters.value(keys.resultFormat, testFormat).toInt();
    index = ui->comboBoxResultFormat->findData(resultFormat);
    ui->comboBoxResultFormat->setCurrentIndex(index);
    xSetupTextFormatValidator(ui->lineEditResultData, resultFormat);

#if 0
    bool bypass = parameters.value(keys.bypass, true).toBool();
    ui->checkBoxBypass->setChecked(bypass);
#endif

    QString testData = parameters.value(keys.testData, defaultTestData).toString();
    ui->lineEditTestData->setText(testData);
}

QMenu *LuaPanel::buttonMenu() const
{
    return m_menu;
}

QByteArray LuaPanel::handleData(const QByteArray &data) const
{
    if (ui->checkBoxBypass->isChecked()) {
        return data;
    } else {
        int type = ui->tabWidgetLua->currentIndex();
        if (type == LuaRunner::ParameterTypeTable) {
            return m_luaRunner->execute(ui->plainTextEditScriptTable->toPlainText(), data, type);
        } else {
            return m_luaRunner->execute(ui->plainTextEditScriptString->toPlainText(), data, type);
        }
    }
}

bool LuaPanel::isBypassed() const
{
    return ui->checkBoxBypass->isChecked();
}

void LuaPanel::onDefaultLuaScriptTriggered()
{
    QFile file(":/res/scripts/lua/default.lua");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString script = file.readAll();
        ui->plainTextEditScriptTable->setPlainText(script);
        file.close();
    }

    ui->tabWidgetLua->setCurrentIndex(0);
}

void LuaPanel::onCheckSumLuaScriptTriggered()
{
    QFile file(":/res/scripts/lua/check_sum.lua");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString script = file.readAll();
        ui->plainTextEditScriptTable->setPlainText(script);
        file.close();
    }

    ui->tabWidgetLua->setCurrentIndex(0);
}

void LuaPanel::onDefaultLuaScriptStringTriggered()
{
    QFile file(":/res/scripts/lua/default_string.lua");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString script = file.readAll();
        ui->plainTextEditScriptString->setPlainText(script);
        file.close();
    }

    ui->tabWidgetLua->setCurrentIndex(1);
}

void LuaPanel::onTestFormatChanged()
{
    int format = ui->comboBoxTestFormat->currentData().toInt();
    xSetupTextFormatValidator(ui->lineEditTestData, format);
    ui->lineEditTestData->setText(xBytes2string(m_testData, format));
}

void LuaPanel::onResultFormatChanged()
{
    int format = ui->comboBoxResultFormat->currentData().toInt();
    xSetupTextFormatValidator(ui->lineEditResultData, format);
    ui->lineEditResultData->setText(xBytes2string(m_resultData, format));
}

void LuaPanel::onTestButtonClicked()
{
    ui->labelInfo->clear();

    int type = ui->tabWidgetLua->currentIndex();
    QString script;
    if (type == LuaRunner::ParameterTypeTable) {
        script = ui->plainTextEditScriptTable->toPlainText();
    } else {
        script = ui->plainTextEditScriptString->toPlainText();
    }

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
