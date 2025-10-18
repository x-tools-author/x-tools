/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sumcheckerui.h"
#include "ui_sumcheckerui.h"

#include "sumchecker.h"

SumCheckerUi::SumCheckerUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::SumCheckerUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);
    ui->comboBoxBytes->addItem("1", SumChecker::SumBytes1);
    ui->comboBoxBytes->addItem("2", SumChecker::SumBytes2);
    ui->comboBoxBytes->addItem("4", SumChecker::SumBytes4);
    ui->comboBoxBytes->addItem("8", SumChecker::SumBytes8);
    ui->checkBoxBigEndian->setChecked(true);
}

SumCheckerUi::~SumCheckerUi()
{
    delete ui;
}

QJsonObject SumCheckerUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    SumCheckerParametersKeys keys;
    parameters[keys.resultBytes] = ui->comboBoxBytes->currentData().toInt();
    parameters[keys.bigEndian] = ui->checkBoxBigEndian->isChecked();
    return parameters;
}

void SumCheckerUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    SumCheckerParametersKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();
    int resultBytes = parametersMap.value(keys.resultBytes, SumChecker::SumBytes1).toInt();
    ui->comboBoxBytes->setCurrentIndex(ui->comboBoxBytes->findData(resultBytes));
    ui->checkBoxBigEndian->setChecked(parametersMap.value(keys.bigEndian, true).toBool());
}
