/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsResponserToolUiEditor.h"
#include "ui_xToolsResponserToolUiEditor.h"

#include "xToolsResponserTool.h"

xToolsResponserToolUiEditor::xToolsResponserToolUiEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::xToolsResponserToolUiEditor)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle(tr("Responser Item Editor"));

    connect(ui->pushButtonOk, &QPushButton::clicked, this, &xToolsResponserToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &xToolsResponserToolUiEditor::reject);
}

xToolsResponserToolUiEditor::~xToolsResponserToolUiEditor()
{
    delete ui;
}

QJsonObject xToolsResponserToolUiEditor::parameters()
{
    bool enable = ui->checkBoxEnable->isChecked();
    QString description = ui->lineEditDescription->text().trimmed();
    int option = ui->comboBoxOption->currentData().toInt();

    int refFormat = ui->comboBoxRefFormat->currentData().toInt();
    int refEscape = ui->comboBoxRefEscape->currentData().toInt();
    int refPrefix = ui->comboBoxRefPrefix->currentData().toInt();
    int refSuffix = ui->comboBoxRefSuffix->currentData().toInt();
    bool refCrcEnable = ui->checkBoxRefCrcEnable->isChecked();
    bool refCrcBigEndian = ui->checkBoxRefCrcBigEndian->isChecked();
    int refAlgorithm = ui->comboBoxRefAlgorithm->currentData().toInt();
    int refStart = ui->spinBoxRefStart->value();
    int refEnd = ui->spinBoxRefEnd->value();
    QString refData = ui->lineEditRefData->text();

    int resFormat = ui->comboBoxResFormat->currentData().toInt();
    int resEscape = ui->comboBoxResEscape->currentData().toInt();
    int resPrefix = ui->comboBoxResPrefix->currentData().toInt();
    int resSuffix = ui->comboBoxResSuffix->currentData().toInt();
    bool resCrcEnable = ui->checkBoxResCrcEnable->isChecked();
    bool resCrcBigEndian = ui->checkBoxResCrcBigEndian->isChecked();
    int resAlgorithm = ui->comboBoxResAlgorithm->currentData().toInt();
    int resStart = ui->spinBoxResStart->value();
    int resEnd = ui->spinBoxResEnd->value();
    int resDelay = ui->spinBoxResDelay->value();
    QString resData = ui->lineEditResData->text();

    xToolsResponserTool::ResponserItemKeys keys;
    QJsonObject jsonObj;
    jsonObj.insert(keys.itemEnable, enable);
    jsonObj.insert(keys.itemDescription, description);
    jsonObj.insert(keys.itemOption, option);

    jsonObj.insert(keys.itemReferenceTextFormat, refFormat);
    jsonObj.insert(keys.itemReferenceEscapeCharacter, refEscape);
    jsonObj.insert(keys.itemReferencePrefix, refPrefix);
    jsonObj.insert(keys.itemReferenceSuffix, refSuffix);
    jsonObj.insert(keys.itemReferenceCrcEnable, refCrcEnable);
    jsonObj.insert(keys.itemReferenceCrcBigEndian, refCrcBigEndian);
    jsonObj.insert(keys.itemReferenceCrcAlgorithm, refAlgorithm);
    jsonObj.insert(keys.itemReferenceCrcStartIndex, refStart);
    jsonObj.insert(keys.itemReferenceCrcEndIndex, refEnd);
    jsonObj.insert(keys.itemReferenceText, refData);

    jsonObj.insert(keys.itemResponseTextFormat, resFormat);
    jsonObj.insert(keys.itemResponseEscapeCharacter, resEscape);
    jsonObj.insert(keys.itemResponsePrefix, resPrefix);
    jsonObj.insert(keys.itemResponseSuffix, resSuffix);
    jsonObj.insert(keys.itemResponseCrcEnable, resCrcEnable);
    jsonObj.insert(keys.itemResponseCrcBigEndian, resCrcBigEndian);
    jsonObj.insert(keys.itemResponseCrcAlgorithm, resAlgorithm);
    jsonObj.insert(keys.itemResponseCrcStartIndex, resStart);
    jsonObj.insert(keys.itemResponseCrcEndIndex, resEnd);
    jsonObj.insert(keys.itemResponseDelay, resDelay);
    jsonObj.insert(keys.itemResponseText, resData);

    return jsonObj;
}

void xToolsResponserToolUiEditor::setParameters(const QJsonObject &params)
{
    xToolsResponserTool::ResponserItemKeys keys;
    bool enable = params.value(keys.itemEnable).toBool();
    QString description = params.value(keys.itemDescription).toString();
    int option = params.value(keys.itemOption).toInt();

    int refFormat = params.value(keys.itemReferenceTextFormat).toInt();
    int refEscape = params.value(keys.itemReferenceEscapeCharacter).toInt();
    int refPrefix = params.value(keys.itemReferencePrefix).toInt();
    int refSuffix = params.value(keys.itemReferenceSuffix).toInt();
    bool refCrcEnable = params.value(keys.itemReferenceCrcEnable).toBool();
    bool refCrcBigEndian = params.value(keys.itemReferenceCrcBigEndian).toBool();
    int refAlgorithm = params.value(keys.itemReferenceCrcAlgorithm).toInt();
    int refStart = params.value(keys.itemReferenceCrcStartIndex).toInt();
    int refEnd = params.value(keys.itemReferenceCrcEndIndex).toInt();
    QString refData = params.value(keys.itemReferenceText).toString();

    int resFormat = params.value(keys.itemResponseTextFormat).toInt();
    int resEscape = params.value(keys.itemResponseEscapeCharacter).toInt();
    int resPrefix = params.value(keys.itemResponsePrefix).toInt();
    int resSuffix = params.value(keys.itemResponseSuffix).toInt();
    bool resCrcEnable = params.value(keys.itemResponseCrcEnable).toBool();
    bool resCrcBigEndian = params.value(keys.itemResponseCrcBigEndian).toBool();
    int resAlgorithm = params.value(keys.itemResponseCrcAlgorithm).toInt();
    int resStart = params.value(keys.itemResponseCrcStartIndex).toInt();
    int resEnd = params.value(keys.itemResponseCrcEndIndex).toInt();
    int resDelay = params.value(keys.itemResponseDelay).toInt();
    QString resData = params.value(keys.itemResponseText).toString();

    ui->checkBoxEnable->setChecked(enable);
    ui->lineEditDescription->setText(description);
    ui->comboBoxOption->setCurrentIndexFromData(option);

    ui->comboBoxRefFormat->setCurrentIndexFromData(refFormat);
    ui->comboBoxRefEscape->setCurrentIndexFromData(refEscape);
    ui->comboBoxRefPrefix->setCurrentIndexFromData(refPrefix);
    ui->comboBoxRefSuffix->setCurrentIndexFromData(refSuffix);
    ui->checkBoxRefCrcEnable->setChecked(refCrcEnable);
    ui->checkBoxRefCrcBigEndian->setChecked(refCrcBigEndian);
    ui->comboBoxRefAlgorithm->setCurrentIndexFromData(refAlgorithm);
    ui->spinBoxRefStart->setValue(refStart);
    ui->spinBoxRefEnd->setValue(refEnd);
    ui->lineEditRefData->setText(refData);

    ui->comboBoxResFormat->setCurrentIndexFromData(resFormat);
    ui->comboBoxResEscape->setCurrentIndexFromData(resEscape);
    ui->comboBoxResPrefix->setCurrentIndexFromData(resPrefix);
    ui->comboBoxResSuffix->setCurrentIndexFromData(resSuffix);
    ui->checkBoxResCrcEnable->setChecked(resCrcEnable);
    ui->checkBoxResCrcBigEndian->setChecked(resCrcBigEndian);
    ui->comboBoxResAlgorithm->setCurrentIndexFromData(resAlgorithm);
    ui->spinBoxResStart->setValue(resStart);
    ui->spinBoxResEnd->setValue(resEnd);
    ui->spinBoxResDelay->setValue(resDelay);
    ui->lineEditResData->setText(resData);
}
