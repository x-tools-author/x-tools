/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "DataEditor.h"
#include "ui_DataEditor.h"

#include "../xIO.h"

namespace xTools {

DataEditor::DataEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataEditor)
{
    ui->setupUi(this);

    xIO::setupAddition(ui->comboBoxPrefix);
    xIO::setupAddition(ui->comboBoxSuffix);
    xIO::setupEscapeCharacter(ui->comboBoxEscapeCharacter);
    xIO::setupCrcAlgorithm(ui->comboBoxAlgorithm);
}

DataEditor::~DataEditor()
{
    delete ui;
}

QJsonObject DataEditor::save() const
{
    QString text = ui->lineEditInput->text();
    int prefix = ui->comboBoxPrefix->currentData().toInt();
    int suffix = ui->comboBoxSuffix->currentData().toInt();
    int escapeCharacter = ui->comboBoxEscapeCharacter->currentData().toInt();

    bool crcEnable = ui->checkBoxAppend->isChecked();
    bool crcBigEndian = ui->checkBoxBigEndian->isChecked();
    int crcAlgorithm = ui->comboBoxAlgorithm->currentData().toInt();
    int crcStartIndex = ui->spinBoxStartIndex->value();
    int crcEndIndex = ui->spinBoxEndIndex->value();

    xIO::TextItemParameterKeys keys;
    QJsonObject parameters;
    parameters[keys.text] = text;
    parameters[keys.prefix] = prefix;
    parameters[keys.suffix] = suffix;
    parameters[keys.escapeCharacter] = escapeCharacter;
    parameters[keys.crcEnable] = crcEnable;
    parameters[keys.crcBigEndian] = crcBigEndian;
    parameters[keys.crcAlgorithm] = crcAlgorithm;
    parameters[keys.crcStartIndex] = crcStartIndex;
    parameters[keys.crcEndIndex] = crcEndIndex;
    return parameters;
}

void DataEditor::load(const QJsonObject &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    xIO::TextItemParameterKeys keys;
    QString text = parameters.value(keys.text).toString();
    int prefix = parameters.value(keys.prefix).toInt();
    int suffix = parameters.value(keys.suffix).toInt();
    int escapeCharacter = parameters.value(keys.escapeCharacter).toInt();

    bool crcEnable = parameters.value(keys.crcEnable).toBool();
    bool crcBigEndian = parameters.value(keys.crcBigEndian).toBool();
    int crcAlgorithm = parameters.value(keys.crcAlgorithm).toInt();
    int crcStartIndex = parameters.value(keys.crcStartIndex).toInt();
    int crcEndIndex = parameters.value(keys.crcEndIndex).toInt();

    int prefixIndex = ui->comboBoxPrefix->findData(prefix);
    int suffixIndex = ui->comboBoxSuffix->findData(suffix);
    int escapeCharacterIndex = ui->comboBoxEscapeCharacter->findData(escapeCharacter);
    int crcAlgorithmIndex = ui->comboBoxAlgorithm->findData(crcAlgorithm);

    ui->lineEditInput->setText(text);
    ui->comboBoxPrefix->setCurrentIndex(prefixIndex);
    ui->comboBoxSuffix->setCurrentIndex(suffixIndex);
    ui->comboBoxEscapeCharacter->setCurrentIndex(escapeCharacterIndex);
    ui->checkBoxAppend->setChecked(crcEnable);
    ui->checkBoxBigEndian->setChecked(crcBigEndian);
    ui->comboBoxAlgorithm->setCurrentIndex(crcAlgorithmIndex);
    ui->spinBoxStartIndex->setValue(crcStartIndex);
    ui->spinBoxEndIndex->setValue(crcEndIndex);
}

} // namespace xTools
