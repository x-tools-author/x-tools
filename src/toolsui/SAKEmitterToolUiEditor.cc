/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKEmitterTool.hh"
#include "SAKEmitterToolUiEditor.hh"
#include "ui_SAKEmitterToolUiEditor.h"

SAKEmitterToolUiEditor::SAKEmitterToolUiEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::SAKEmitterToolUiEditor)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle(tr("Emitter Item Editor"));

    connect(ui->pushButtonApply, &QPushButton::clicked,
            this, &SAKEmitterToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &SAKEmitterToolUiEditor::reject);
}

SAKEmitterToolUiEditor::~SAKEmitterToolUiEditor()
{
    delete ui;
}

QJsonObject SAKEmitterToolUiEditor::parameters()
{
    return QJsonObject();
}

void SAKEmitterToolUiEditor::setParameters(const QJsonObject &params)
{
    SAKEmitterTableModel::EDEmitterDataKeys keys;
    bool enable = params.value(keys.itemEnable).toBool();
    QString description = params.value(keys.itemDescription).toString();
    int format = params.value(keys.itemTextFormat).toInt();
    int escape = params.value(keys.itemEscapeCharacter).toInt();
    int prefix = params.value(keys.itemPrefix).toInt();
    int suffix = params.value(keys.itemSuffix).toInt();
    int interval = params.value(keys.itemInterval).toInt();
    bool crcEnable = params.value(keys.itemCrcEnable).toBool();
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
    ui->comboBoxAlgorithm->setCurrentIndexFromData(algotirhm);
    ui->spinBoxStartUbdex->setValue(start);
    ui->spinBoxEndIndex->setValue(end);
    ui->lineEditData->setText(data);
}
