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

#include "common/luarunner.h"
#include "common/xtools.h"

struct LuaPanelDataKeys
{
    const QString Script = "script";
    const QString TestFormat = "testFormat";
    const QString ResultFormat = "resultFormat";
    const QString Bypass = "bypass";
    const QString testData = "testData";
};

LuaPanel::LuaPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::LuaPanel)
    , m_menu(nullptr)
{
    ui->setupUi(this);

    m_menu = new QMenu(this);
    m_menu->addAction(tr("Default Lua Script"), this, &LuaPanel::onDefaultLuaScriptTriggered);
    m_menu->addAction(tr("Checksum Lua Script"), this, &LuaPanel::onCheckSumLuaScriptTriggered);
    onDefaultLuaScriptTriggered();

    m_luaRunner = new LuaRunner(this);
    m_luaRunner->start();

    connect(ui->pushButtonTest, &QPushButton::clicked, this, &LuaPanel::onTestButtonClicked);
    connect(ui->comboBoxTestFormat, xComboBoxActivated, this, &LuaPanel::onTestFormatChanged);
    connect(ui->comboBoxResultFormat, xComboBoxActivated, this, &LuaPanel::onResultFormatChanged);
    setupTextFormat(ui->comboBoxTestFormat);
    setupTextFormat(ui->comboBoxResultFormat);
    onTestFormatChanged();
    onResultFormatChanged();
}

LuaPanel::~LuaPanel()
{
    delete ui;
}

QVariantMap LuaPanel::save() const
{
    QVariantMap map = Panel::save();
    LuaPanelDataKeys keys;
    map[keys.Script] = ui->plainTextEditScript->toPlainText();
    map[keys.TestFormat] = ui->comboBoxTestFormat->currentData().toInt();
    map[keys.ResultFormat] = ui->comboBoxResultFormat->currentData().toInt();
    map[keys.Bypass] = ui->checkBoxBypass->isChecked();
    map[keys.testData] = ui->lineEditTestData->text();
    return map;
}

void LuaPanel::load(const QVariantMap &parameters)
{
    Panel::load(parameters);

    int defaultTestFormat = static_cast<int>(TextFormat::Hex);
    QString defaultTestData = bytes2string(m_testData, defaultTestFormat);
    LuaPanelDataKeys keys;
    ui->plainTextEditScript->setPlainText(parameters[keys.Script].toString());
    if (ui->plainTextEditScript->toPlainText().isEmpty()) {
        onDefaultLuaScriptTriggered();
    }

    int testFormat = parameters.value(keys.TestFormat, defaultTestFormat).toInt();
    int index = ui->comboBoxTestFormat->findData(testFormat);
    ui->comboBoxTestFormat->setCurrentIndex(index);
    setupTextFormatValidator(ui->lineEditTestData, testFormat);

    int resultFormat = parameters.value(keys.ResultFormat, testFormat).toInt();
    index = ui->comboBoxResultFormat->findData(resultFormat);
    ui->comboBoxResultFormat->setCurrentIndex(index);
    setupTextFormatValidator(ui->lineEditResultData, resultFormat);

    bool bypass = parameters.value(keys.Bypass, false).toBool();
    ui->checkBoxBypass->setChecked(bypass);

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
        return m_luaRunner->execute(ui->plainTextEditScript->toPlainText(), data);
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
        ui->plainTextEditScript->setPlainText(script);
        file.close();
    }
}

void LuaPanel::onCheckSumLuaScriptTriggered()
{
    QFile file(":/res/scripts/lua/check_sum.lua");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString script = file.readAll();
        ui->plainTextEditScript->setPlainText(script);
        file.close();
    }
}

void LuaPanel::onTestFormatChanged()
{
    int format = ui->comboBoxTestFormat->currentData().toInt();
    setupTextFormatValidator(ui->lineEditTestData, format);
    ui->lineEditTestData->setText(bytes2string(m_testData, format));
}

void LuaPanel::onResultFormatChanged()
{
    int format = ui->comboBoxResultFormat->currentData().toInt();
    setupTextFormatValidator(ui->lineEditResultData, format);
    ui->lineEditResultData->setText(bytes2string(m_resultData, format));
}

void LuaPanel::onTestButtonClicked()
{
    ui->labelInfo->clear();

    QString script = ui->plainTextEditScript->toPlainText();
    int testFormat = ui->comboBoxTestFormat->currentData().toInt();
    QByteArray data = string2bytes(ui->lineEditTestData->text(), testFormat);
    int resultFormat = ui->comboBoxResultFormat->currentData().toInt();
    m_resultData = m_luaRunner->execute(script, data);
    QString str = bytes2string(m_resultData, resultFormat);
    ui->lineEditResultData->setText(str);

    if (m_resultData.isEmpty()) {
        QString errStr = m_luaRunner->error();
        ui->labelInfo->setText(errStr);
    }
}