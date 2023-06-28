/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKResponserTool.hh"
#include "SAKResponserToolUiEditor.hh"
#include "ui_SAKResponserToolUiEditor.h"

SAKResponserToolUiEditor::SAKResponserToolUiEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::SAKResponserToolUiEditor)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle(tr("Responser Item Editor"));

    connect(ui->pushButtonOk, &QPushButton::clicked,
            this, &SAKResponserToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &SAKResponserToolUiEditor::reject);
}

SAKResponserToolUiEditor::~SAKResponserToolUiEditor()
{
    delete ui;
}

QJsonObject SAKResponserToolUiEditor::parameters()
{
    bool enable = ui->checkBoxEnable->isChecked();
    QString description = ui->lineEditDescription->text().trimmed();
    int option = ui->comboBoxOption->currentData().toInt();

    int     refFormat   = ui->comboBoxRefFormat->currentData().toInt();
    int     refEscape   = ui->comboBoxRefEscape->currentData().toInt();
    int     refPrefix   = ui->comboBoxRefPrefix->currentData().toInt();
    int     refSuffix   = ui->comboBoxRefSuffix->currentData().toInt();
    bool    refCrcEnable= ui->checkBoxRefCrcEnable->isChecked();
    int     refAlgorithm= ui->comboBoxRefAlgorithm->currentData().toInt();
    int     refStart    = ui->spinBoxRefStart->value();
    int     refEnd      = ui->spinBoxRefEnd->value();
    QString refData     = ui->lineEditRefData->text();

    int     resFormat   = ui->comboBoxResFormat->currentData().toInt();
    int     resEscape   = ui->comboBoxResEscape->currentData().toInt();
    int     resPrefix   = ui->comboBoxResPrefix->currentData().toInt();
    int     resSuffix   = ui->comboBoxResSuffix->currentData().toInt();
    bool    resCrcEnable= ui->checkBoxResCrcEnable->isChecked();
    int     resAlgorithm= ui->comboBoxResAlgorithm->currentData().toInt();
    int     resStart    = ui->spinBoxResStart->value();
    int     resEnd      = ui->spinBoxResEnd->value();
    QString resData     = ui->lineEditResData->text();

    SAKResponserTool::ResponserItemKeys keys;
    QJsonObject jsonObj;
    jsonObj.insert(keys.itemEnable, enable);
    jsonObj.insert(keys.itemDescription, description);
    jsonObj.insert(keys.itemOption, option);

    jsonObj.insert(keys.itemReferenceTextFormat, refFormat);
    jsonObj.insert(keys.itemReferenceEscapeCharacter, refEscape);
    jsonObj.insert(keys.itemReferencePrefix, refPrefix);
    jsonObj.insert(keys.itemReferenceSuffix, refSuffix);
    jsonObj.insert(keys.itemReferenceCrcEnable, refCrcEnable);
    jsonObj.insert(keys.itemReferenceCrcAlgorithm, refAlgorithm);
    jsonObj.insert(keys.itemReferenceCrcStartIndex, refStart);
    jsonObj.insert(keys.itemReferenceCrcEndIndex, refEnd);
    jsonObj.insert(keys.itemReferenceText, refData);

    jsonObj.insert(keys.itemResponseTextFormat, resFormat);
    jsonObj.insert(keys.itemResponseEscapeCharacter, resEscape);
    jsonObj.insert(keys.itemResponsePrefix, resPrefix);
    jsonObj.insert(keys.itemResponseSuffix, resSuffix);
    jsonObj.insert(keys.itemResponseCrcEnable, resCrcEnable);
    jsonObj.insert(keys.itemResponseCrcAlgorithm, resAlgorithm);
    jsonObj.insert(keys.itemResponseCrcStartIndex, resStart);
    jsonObj.insert(keys.itemResponseCrcEndIndex, resEnd);
    jsonObj.insert(keys.itemResponseText, resData);

    return jsonObj;
}

void SAKResponserToolUiEditor::setParameters(const QJsonObject &params)
{
    SAKResponserTool::ResponserItemKeys keys;
    bool enable = params.value(keys.itemEnable).toBool();
    QString description = params.value(keys.itemDescription).toString();
    int option = params.value(keys.itemOption).toInt();

    int     refFormat   = params.value(keys.itemReferenceTextFormat).toInt();
    int     refEscape   = params.value(keys.itemReferenceEscapeCharacter).toInt();
    int     refPrefix   = params.value(keys.itemReferencePrefix).toInt();
    int     refSuffix   = params.value(keys.itemReferenceSuffix).toInt();
    bool    refCrcEnable= params.value(keys.itemReferenceCrcEnable).toBool();
    int     refAlgorithm= params.value(keys.itemReferenceCrcAlgorithm).toInt();
    int     refStart    = params.value(keys.itemReferenceCrcStartIndex).toInt();
    int     refEnd      = params.value(keys.itemReferenceCrcEndIndex).toInt();
    QString refData     = params.value(keys.itemReferenceText).toString();

    int     resFormat   = params.value(keys.itemResponseTextFormat).toInt();
    int     resEscape   = params.value(keys.itemResponseEscapeCharacter).toInt();
    int     resPrefix   = params.value(keys.itemResponsePrefix).toInt();
    int     resSuffix   = params.value(keys.itemResponseSuffix).toInt();
    bool    resCrcEnable= params.value(keys.itemResponseCrcEnable).toBool();
    int     resAlgorithm= params.value(keys.itemResponseCrcAlgorithm).toInt();
    int     resStart    = params.value(keys.itemResponseCrcStartIndex).toInt();
    int     resEnd      = params.value(keys.itemResponseCrcEndIndex).toInt();
    QString resData     = params.value(keys.itemResponseText).toString();

    ui->checkBoxEnable->setChecked(enable);
    ui->lineEditDescription->setText(description);
    ui->comboBoxOption->setCurrentIndexFromData(option);

    ui->comboBoxRefFormat->setCurrentIndexFromData(refFormat);
    ui->comboBoxRefEscape->setCurrentIndexFromData(refEscape);
    ui->comboBoxRefPrefix->setCurrentIndexFromData(refPrefix);
    ui->comboBoxRefSuffix->setCurrentIndexFromData(refSuffix);
    ui->checkBoxRefCrcEnable->setChecked(refCrcEnable);
    ui->comboBoxRefAlgorithm->setCurrentIndexFromData(refAlgorithm);
    ui->spinBoxRefStart->setValue(refStart);
    ui->spinBoxRefEnd->setValue(refEnd);
    ui->lineEditRefData->setText(refData);

    ui->comboBoxResFormat->setCurrentIndexFromData(resFormat);
    ui->comboBoxResEscape->setCurrentIndexFromData(resEscape);
    ui->comboBoxResPrefix->setCurrentIndexFromData(resPrefix);
    ui->comboBoxResSuffix->setCurrentIndexFromData(resSuffix);
    ui->checkBoxResCrcEnable->setChecked(resCrcEnable);
    ui->comboBoxResAlgorithm->setCurrentIndexFromData(resAlgorithm);
    ui->spinBoxResStart->setValue(resStart);
    ui->spinBoxResEnd->setValue(resEnd);
    ui->lineEditResData->setText(resData);
}
