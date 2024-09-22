/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TextItemEditor.h"
#include "ui_TextItemEditor.h"

#include "../xIO.h"
#include "CRC.h "

namespace xTools {

TextItemEditor::TextItemEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TextItemEditor)
{
    ui->setupUi(this);

    xIO::setupAddition(ui->comboBoxPrefix);
    xIO::setupAddition(ui->comboBoxSuffix);
    xIO::setupEscapeCharacter(ui->comboBoxEscapeCharacter);
    CRC::setupAlgorithm(ui->comboBoxAlgorithm);
    xIO::setupTextFormat(ui->comboBoxFormat);

    connect(ui->comboBoxFormat,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TextItemEditor::onTextFormatChanged);
}

TextItemEditor::~TextItemEditor()
{
    delete ui;
}

QJsonObject TextItemEditor::save() const
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
    int format = ui->comboBoxFormat->currentData().toInt();

    xIO::TextItemKeys keys;
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
    parameters[keys.textFormat] = format;
    return parameters;
}

void TextItemEditor::load(const QJsonObject &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    xIO::TextItemKeys keys;
    QString text = parameters.value(keys.text).toString();
    int format = parameters.value(keys.textFormat).toInt();
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

    ui->comboBoxPrefix->setCurrentIndex(prefixIndex);
    ui->comboBoxSuffix->setCurrentIndex(suffixIndex);
    ui->comboBoxEscapeCharacter->setCurrentIndex(escapeCharacterIndex);
    ui->checkBoxAppend->setChecked(crcEnable);
    ui->checkBoxBigEndian->setChecked(crcBigEndian);
    ui->comboBoxAlgorithm->setCurrentIndex(crcAlgorithmIndex);
    ui->spinBoxStartIndex->setValue(crcStartIndex);
    ui->spinBoxEndIndex->setValue(crcEndIndex);
    ui->comboBoxFormat->setCurrentIndex(ui->comboBoxFormat->findData(format));
    ui->lineEditInput->setText(text);
}

void TextItemEditor::onTextFormatChanged()
{
    ui->lineEditInput->clear();
    int format = ui->comboBoxFormat->currentData().toInt();
    xIO::setupTextFormatValidator(ui->lineEditInput, format);
}

} // namespace xTools
