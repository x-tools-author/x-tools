/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsPrestorerToolUiEditor.h"
#include "ui_xToolsPrestorerToolUiEditor.h"

#include "xToolsPrestorerTool.h"


xToolsPrestorerToolUiEditor::xToolsPrestorerToolUiEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::xToolsPrestorerToolUiEditor)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle(tr("Prestorer Item Editor"));

    connect(ui->pushButtonOk, &QPushButton::clicked, this, &xToolsPrestorerToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &xToolsPrestorerToolUiEditor::reject);
}

xToolsPrestorerToolUiEditor::~xToolsPrestorerToolUiEditor()
{
    delete ui;
}

QJsonObject xToolsPrestorerToolUiEditor::parameters()
{
    QString description = ui->lineEditDescription->text().trimmed();
    int format = ui->comboBoxFormat->currentData().toInt();
    int escape = ui->comboBoxEscape->currentData().toInt();
    int prefix = ui->comboBoxPrefix->currentData().toInt();
    int suffix = ui->comboBoxSuffix->currentData().toInt();
    bool crcEnable = ui->checkBoxCrc->isChecked();
    bool bigEndian = ui->checkBoxBigEndian->isChecked();
    int start = ui->spinBoxStart->value();
    int end = ui->spinBoxEnd->value();
    int algorithm = ui->comboBoxAlgorithm->currentData().toInt();
    QString data = ui->lineEditData->text();

    QJsonObject params;
    xToolsPrestorerTool::ItemKeys keys;
    params.insert(keys.itemDescription, description);
    params.insert(keys.itemTextFormat, format);
    params.insert(keys.itemEscapeCharacter, escape);
    params.insert(keys.itemPrefix, prefix);
    params.insert(keys.itemSuffix, suffix);
    params.insert(keys.itemCrcEnable, crcEnable);
    params.insert(keys.itemCrcBigEndian, bigEndian);
    params.insert(keys.itemCrcStartIndex, start);
    params.insert(keys.itemCrcEndIndex, end);
    params.insert(keys.itemCrcAlgorithm, algorithm);
    params.insert(keys.itemText, data);

    return params;
}

void xToolsPrestorerToolUiEditor::setParameters(const QJsonObject &params)
{
    xToolsPrestorerTool::ItemKeys keys;
    QString description = params.value(keys.itemDescription).toString();
    int format = params.value(keys.itemTextFormat).toInt();
    int escape = params.value(keys.itemEscapeCharacter).toInt();
    int prefix = params.value(keys.itemPrefix).toInt();
    int suffix = params.value(keys.itemSuffix).toInt();
    bool crcEnable = params.value(keys.itemCrcEnable).toBool();
    bool crcBigEndian = params.value(keys.itemCrcBigEndian).toBool();
    int start = params.value(keys.itemCrcStartIndex).toInt();
    int end = params.value(keys.itemCrcEndIndex).toInt();
    QString data = params.value(keys.itemText).toString();

    ui->lineEditDescription->setText(description);
    ui->comboBoxFormat->setCurrentIndexFromData(format);
    ui->comboBoxEscape->setCurrentIndexFromData(escape);
    ui->comboBoxPrefix->setCurrentIndexFromData(prefix);
    ui->comboBoxSuffix->setCurrentIndexFromData(suffix);
    ui->checkBoxCrc->setChecked(crcEnable);
    ui->checkBoxBigEndian->setChecked(crcBigEndian);
    ui->spinBoxStart->setValue(start);
    ui->spinBoxEnd->setValue(end);
    ui->lineEditData->setText(data);
}
