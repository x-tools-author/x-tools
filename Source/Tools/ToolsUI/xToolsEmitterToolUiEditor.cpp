/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsEmitterToolUiEditor.h"
#include "ui_xToolsEmitterToolUiEditor.h"

#include "xToolsEmitterTool.h"

xToolsEmitterToolUiEditor::xToolsEmitterToolUiEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::xToolsEmitterToolUiEditor)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle(tr("Emitter Item Editor"));

    connect(ui->pushButtonApply, &QPushButton::clicked, this, &xToolsEmitterToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &xToolsEmitterToolUiEditor::reject);
}

xToolsEmitterToolUiEditor::~xToolsEmitterToolUiEditor()
{
    delete ui;
}

QJsonObject xToolsEmitterToolUiEditor::parameters()
{
    bool enable = ui->checkBoxEnable->isChecked();
    QString description = ui->lineEditDescription->text().trimmed();
    int format = ui->comboBoxFormat->currentData().toInt();
    int escape = ui->comboBoxEscaoe->currentData().toInt();
    int prefix = ui->comboBoxPrefix->currentData().toInt();
    int suffix = ui->comboBoxSufix->currentData().toInt();
    int interval = ui->spinBoxInterval->value();
    bool crcEnable = ui->checkBoxCrcEnable->isChecked();
    bool crcBigEndian = ui->checkBoxBigEndian->isChecked();
    int algotirhm = ui->comboBoxAlgorithm->currentData().toInt();
    int start = ui->spinBoxStartIndex->value();
    int end = ui->spinBoxEndIndex->value();
    QString data = ui->lineEditData->text();

    xToolsEmitterTool::DataKeys keys;
    QJsonObject params;
    params.insert(keys.itemEnable, enable);
    params.insert(keys.itemDescription, description);
    params.insert(keys.itemTextFormat, format);
    params.insert(keys.itemEscapeCharacter, escape);
    params.insert(keys.itemPrefix, prefix);
    params.insert(keys.itemSuffix, suffix);
    params.insert(keys.itemInterval, interval);
    params.insert(keys.itemCrcEnable, crcEnable);
    params.insert(keys.itemCrcBigEndian, crcBigEndian);
    params.insert(keys.itemCrcAlgorithm, algotirhm);
    params.insert(keys.itemCrcStartIndex, start);
    params.insert(keys.itemCrcEndIndex, end);
    params.insert(keys.itemText, data);
    return params;
}

void xToolsEmitterToolUiEditor::setParameters(const QJsonObject &params)
{
    xToolsEmitterTool::DataKeys keys;
    bool enable = params.value(keys.itemEnable).toBool();
    QString description = params.value(keys.itemDescription).toString();
    int format = params.value(keys.itemTextFormat).toInt();
    int escape = params.value(keys.itemEscapeCharacter).toInt();
    int prefix = params.value(keys.itemPrefix).toInt();
    int suffix = params.value(keys.itemSuffix).toInt();
    int interval = params.value(keys.itemInterval).toInt();
    bool crcEnable = params.value(keys.itemCrcEnable).toBool();
    bool crcBigEndian = params.value(keys.itemCrcBigEndian).toBool();
    int algotirhm = params.value(keys.itemCrcAlgorithm).toInt();
    int start = params.value(keys.itemCrcStartIndex).toInt();
    int end = params.value(keys.itemCrcEndIndex).toInt();
    QString data = params.value(keys.itemText).toString();

    ui->checkBoxEnable->setChecked(enable);
    ui->lineEditDescription->setText(description);
    ui->comboBoxFormat->setCurrentIndexFromData(format);
    ui->comboBoxEscaoe->setCurrentIndexFromData(escape);
    ui->comboBoxPrefix->setCurrentIndexFromData(prefix);
    ui->comboBoxSufix->setCurrentIndexFromData(suffix);
    ui->spinBoxInterval->setValue(interval);
    ui->checkBoxCrcEnable->setChecked(crcEnable);
    ui->checkBoxBigEndian->setChecked(crcBigEndian);
    ui->comboBoxAlgorithm->setCurrentIndexFromData(algotirhm);
    ui->spinBoxStartIndex->setValue(start);
    ui->spinBoxEndIndex->setValue(end);
    ui->lineEditData->setText(data);
}
