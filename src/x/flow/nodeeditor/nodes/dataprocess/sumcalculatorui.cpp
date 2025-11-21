/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sumcalculatorui.h"
#include "ui_sumcalculatorui.h"

#include "sumcalculator.h"

SumCalculatorUi::SumCalculatorUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::SumCalculatorUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    ui->comboBoxBytes->addItem("1", SumCalculator::SumBytes1);
    ui->comboBoxBytes->addItem("2", SumCalculator::SumBytes2);
    ui->comboBoxBytes->addItem("4", SumCalculator::SumBytes4);
    ui->comboBoxBytes->addItem("8", SumCalculator::SumBytes8);
    ui->checkBoxBigEndian->setChecked(true);
}

SumCalculatorUi::~SumCalculatorUi()
{
    delete ui;
}

QJsonObject SumCalculatorUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    SumCalculatorParametersKeys keys;
    parameters[keys.resultBytes] = ui->comboBoxBytes->currentData().toInt();
    parameters[keys.bigEndian] = ui->checkBoxBigEndian->isChecked();
    return parameters;
}

void SumCalculatorUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    SumCalculatorParametersKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();
    int resultBytes = parametersMap.value(keys.resultBytes, SumCalculator::SumBytes1).toInt();
    ui->comboBoxBytes->setCurrentIndex(ui->comboBoxBytes->findData(resultBytes));
    ui->checkBoxBigEndian->setChecked(parametersMap.value(keys.bigEndian, true).toBool());
}
