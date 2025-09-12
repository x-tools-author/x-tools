/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "crccalculatorui.h"
#include "ui_crccalculatorui.h"

#include "common/crc.h"
#include "crccalculator.h"

CrcCalculatorUi::CrcCalculatorUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::CrcCalculatorUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);
    ui->checkBoxBigEndian->setChecked(true);
    CRC::setupAlgorithm(ui->comboBoxArithmetic);
}

CrcCalculatorUi::~CrcCalculatorUi()
{
    delete ui;
}

QJsonObject CrcCalculatorUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    CrcCalculatorParametersKeys keys;
    parameters.insert(keys.startIndex, ui->spinBoxStartIndex->value());
    parameters.insert(keys.endIndex, ui->spinBoxEndIndex->value());
    parameters.insert(keys.algorithm, ui->comboBoxArithmetic->currentData().toInt());
    parameters.insert(keys.bigEndian, ui->checkBoxBigEndian->isChecked());
    return parameters;
}

void CrcCalculatorUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    QVariantMap parametersMap = parameters.toVariantMap();
    CrcCalculatorParametersKeys keys;

    int defaultAlgorithm = static_cast<int>(CRC::Algorithm::CRC_8);
    int algorithm = parametersMap.value(keys.algorithm, defaultAlgorithm).toInt();
    int startIndex = parametersMap.value(keys.startIndex, 0).toInt();
    int endIndex = parametersMap.value(keys.endIndex, 0).toInt();
    bool bigEndian = parametersMap.value(keys.bigEndian, true).toBool();

    ui->spinBoxStartIndex->setValue(startIndex);
    ui->spinBoxEndIndex->setValue(endIndex);
    int index = ui->comboBoxArithmetic->findData(algorithm);
    ui->comboBoxArithmetic->setCurrentIndex(index);
    ui->checkBoxBigEndian->setChecked(bigEndian);
}
