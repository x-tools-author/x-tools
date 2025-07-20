/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "inputsettings.h"
#include "ui_inputsettings.h"

#include "common/crc.h"
#include "common/xtools.h"

InputSettings::InputSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputSettings)
{
    ui->setupUi(this);

    setupAddition(ui->comboBoxPrefix);
    setupAddition(ui->comboBoxSuffix);
    setupEscapeCharacter(ui->comboBoxEscapeCharacter);
    CRC::setupAlgorithm(ui->comboBoxCrcAlgorithm, true);
}

InputSettings::~InputSettings()
{
    delete ui;
}

QVariantMap InputSettings::save()
{
    bool showDataPreview = ui->checkBoxShowDataPreview->isChecked();

    int prefix = ui->comboBoxPrefix->currentData().toInt();
    int suffix = ui->comboBoxSuffix->currentData().toInt();
    int escapeCharacter = ui->comboBoxEscapeCharacter->currentData().toInt();

    int algorithm = ui->comboBoxCrcAlgorithm->currentData().toInt();
    int startIndex = ui->spinBoxCrcStartIndex->value();
    int endIndex = ui->spinBoxCrcEndIndex->value();
    bool appendCrc = ui->checkBoxAppendCrc->isChecked();
    bool bigEndian = ui->checkBoxBigEndian->isChecked();

    QVariantMap map;
    map.insert("showDataPreview", showDataPreview);
    map.insert("prefix", prefix);
    map.insert("suffix", suffix);
    map.insert("escapeCharacter", escapeCharacter);
    map.insert("algorithm", algorithm);
    map.insert("startIndex", startIndex);
    map.insert("endIndex", endIndex);
    map.insert("appendCrc", appendCrc);
    map.insert("bigEndian", bigEndian);
    return map;
}

void InputSettings::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    bool showDataPreview = parameters.value("showDataPreview").toBool();

    int prefix = parameters.value("prefix").toInt();
    int suffix = parameters.value("suffix").toInt();
    int escapeCharacter = parameters.value("escapeCharacter").toInt();

    int algorithm = parameters.value("algorithm").toInt();
    int startIndex = parameters.value("startIndex").toInt();
    int endIndex = parameters.value("endIndex").toInt();
    bool appendCrc = parameters.value("appendCrc").toBool();
    bool bigEndian = parameters.value("bigEndian").toBool();

    // clang-format off
    ui->checkBoxShowDataPreview->setChecked(showDataPreview);

    ui->comboBoxPrefix->setCurrentIndex(ui->comboBoxPrefix->findData(prefix));
    ui->comboBoxSuffix->setCurrentIndex(ui->comboBoxSuffix->findData(suffix));
    ui->comboBoxEscapeCharacter->setCurrentIndex(ui->comboBoxEscapeCharacter->findData(escapeCharacter));

    ui->comboBoxCrcAlgorithm->setCurrentIndex(ui->comboBoxCrcAlgorithm->findData(algorithm));
    ui->spinBoxCrcStartIndex->setValue(startIndex);
    ui->spinBoxCrcEndIndex->setValue(endIndex);
    ui->checkBoxAppendCrc->setChecked(appendCrc);
    ui->checkBoxBigEndian->setChecked(bigEndian);
    // clang-format on
}

InputSettings::Parameters InputSettings::parameters() const
{
    bool showDataPreview = ui->checkBoxShowDataPreview->isChecked();

    int prefix = ui->comboBoxPrefix->currentData().toInt();
    int suffix = ui->comboBoxSuffix->currentData().toInt();
    int escapeCharacter = ui->comboBoxEscapeCharacter->currentData().toInt();

    int algorithm = ui->comboBoxCrcAlgorithm->currentData().toInt();
    int startIndex = ui->spinBoxCrcStartIndex->value();
    int endIndex = ui->spinBoxCrcEndIndex->value();
    bool appendCrc = ui->checkBoxAppendCrc->isChecked();
    bool bigEndian = ui->checkBoxBigEndian->isChecked();

    Parameters params;
    params.showDataPreview = showDataPreview;
    params.prefix = prefix;
    params.suffix = suffix;
    params.escapeCharacter = escapeCharacter;
    params.algorithm = algorithm;
    params.startIndex = startIndex;
    params.endIndex = endIndex;
    params.appendCrc = appendCrc;
    params.bigEndian = bigEndian;
    return params;
}
