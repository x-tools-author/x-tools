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

#include "common/xtools.h"
#include "utilities/crc.h"

struct InputSettingParameterKeys
{
    const QString showDataPreview{"showDataPreview"};
    const QString prefix{"prefix"};
    const QString suffix{"suffix"};
    const QString usingCustomPrefix{"usingCustomPrefix"};
    const QString usingCustomSuffix{"usingCustomSuffix"};
    const QString customPrefix{"customPrefix"};
    const QString customSuffix{"customSuffix"};
    const QString escapeCharacter{"escapeCharacter"};
    const QString algorithm{"algorithm"};
    const QString startIndex{"startIndex"};
    const QString endIndex{"endIndex"};
    const QString appendCrc{"appendCrc"};
    const QString bigEndian{"bigEndian"};
};

InputSettings::InputSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputSettings)
{
    ui->setupUi(this);
    xSetupTextFormatValidator(ui->lineEditPrefix, static_cast<int>(TextFormat::Hex));
    xSetupTextFormatValidator(ui->lineEditSuffix, static_cast<int>(TextFormat::Hex));

    xSetupAddition(ui->comboBoxPrefix);
    xSetupAddition(ui->comboBoxSuffix);
    xSetupEscapeCharacter(ui->comboBoxEscapeCharacter);
    xTools::CRC::setupAlgorithm(ui->comboBoxCrcAlgorithm, true, true);
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
    bool usingCustomPrefix = ui->checkBoxPrefix->isChecked();
    bool usingCustomSuffix = ui->checkBoxSuffix->isChecked();
    QString customPrefix = ui->lineEditPrefix->text();
    QString customSuffix = ui->lineEditSuffix->text();
    int escapeCharacter = ui->comboBoxEscapeCharacter->currentData().toInt();

    int algorithm = ui->comboBoxCrcAlgorithm->currentData().toInt();
    int startIndex = ui->spinBoxCrcStartIndex->value();
    int endIndex = ui->spinBoxCrcEndIndex->value();
    bool appendCrc = ui->checkBoxAppendCrc->isChecked();
    bool bigEndian = ui->checkBoxBigEndian->isChecked();

    InputSettingParameterKeys keys;
    QVariantMap map;
    map.insert(keys.showDataPreview, showDataPreview);
    map.insert(keys.prefix, prefix);
    map.insert(keys.suffix, suffix);
    map.insert(keys.usingCustomPrefix, usingCustomPrefix);
    map.insert(keys.usingCustomSuffix, usingCustomSuffix);
    map.insert(keys.customPrefix, customPrefix);
    map.insert(keys.customSuffix, customSuffix);
    map.insert(keys.escapeCharacter, escapeCharacter);
    map.insert(keys.algorithm, algorithm);
    map.insert(keys.startIndex, startIndex);
    map.insert(keys.endIndex, endIndex);
    map.insert(keys.appendCrc, appendCrc);
    map.insert(keys.bigEndian, bigEndian);
    return map;
}

void InputSettings::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    InputSettingParameterKeys keys;
    bool showDataPreview = parameters.value(keys.showDataPreview).toBool();

    int prefix = parameters.value(keys.prefix).toInt();
    int suffix = parameters.value(keys.suffix).toInt();
    bool usingCustomPrefix = parameters.value(keys.usingCustomPrefix).toBool();
    bool usingCustomSuffix = parameters.value(keys.usingCustomSuffix).toBool();
    QString customPrefix = parameters.value(keys.customPrefix).toString();
    QString customSuffix = parameters.value(keys.customSuffix).toString();
    int escapeCharacter = parameters.value(keys.escapeCharacter).toInt();

    int algorithm = parameters.value(keys.algorithm).toInt();
    int startIndex = parameters.value(keys.startIndex).toInt();
    int endIndex = parameters.value(keys.endIndex).toInt();
    bool appendCrc = parameters.value(keys.appendCrc).toBool();
    bool bigEndian = parameters.value(keys.bigEndian).toBool();

    // clang-format off
    ui->checkBoxShowDataPreview->setChecked(showDataPreview);

    ui->comboBoxPrefix->setCurrentIndex(ui->comboBoxPrefix->findData(prefix));
    ui->comboBoxSuffix->setCurrentIndex(ui->comboBoxSuffix->findData(suffix));
    ui->comboBoxEscapeCharacter->setCurrentIndex(ui->comboBoxEscapeCharacter->findData(escapeCharacter));

    ui->checkBoxPrefix->setChecked(usingCustomPrefix);
    ui->checkBoxSuffix->setChecked(usingCustomSuffix);
    ui->lineEditPrefix->setText(customPrefix);
    ui->lineEditSuffix->setText(customSuffix);

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

    bool usingCustomPrefix = ui->checkBoxPrefix->isChecked();
    bool usingCustomSuffix = ui->checkBoxSuffix->isChecked();
    QByteArray customPrefix = QByteArray::fromHex(ui->lineEditPrefix->text().trimmed().toLatin1());
    QByteArray customSuffix = QByteArray::fromHex(ui->lineEditSuffix->text().trimmed().toLatin1());

    int algorithm = ui->comboBoxCrcAlgorithm->currentData().toInt();
    int startIndex = ui->spinBoxCrcStartIndex->value();
    int endIndex = ui->spinBoxCrcEndIndex->value();
    bool appendCrc = ui->checkBoxAppendCrc->isChecked();
    bool bigEndian = ui->checkBoxBigEndian->isChecked();

    Parameters params;
    params.showDataPreview = showDataPreview;
    params.prefix = prefix;
    params.suffix = suffix;
    params.usingCustomPrefix = usingCustomPrefix;
    params.usingCustomSuffix = usingCustomSuffix;
    params.customPrefix = customPrefix;
    params.customSuffix = customSuffix;
    params.escapeCharacter = escapeCharacter;
    params.algorithm = algorithm;
    params.startIndex = startIndex;
    params.endIndex = endIndex;
    params.appendCrc = appendCrc;
    params.bigEndian = bigEndian;
    return params;
}
