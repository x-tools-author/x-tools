/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "filterui.h"
#include "ui_filterui.h"

#include "common/xtools.h"
#include "filter.h"

FilterUi::FilterUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::FilterUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    connect(ui->comboBoxFormat, &QComboBox::currentIndexChanged, this, [=]() {
        int format = ui->comboBoxFormat->currentData().toInt();
        xSetupTextFormatValidator(ui->lineEditReference, format);
    });

    ResponseOption option = ResponseOption::InputEqualReference;
    ui->comboBoxOption->addItem(xResponseOptionName(option), static_cast<int>(option));
    option = ResponseOption::InputContainReference;
    ui->comboBoxOption->addItem(xResponseOptionName(option), static_cast<int>(option));
    option = ResponseOption::InputDoesNotContainReference;
    ui->comboBoxOption->addItem(xResponseOptionName(option), static_cast<int>(option));

    xSetupTextFormat(ui->comboBoxFormat);
    xSetupEscapeCharacter(ui->comboBoxEscape);
}

FilterUi::~FilterUi()
{
    delete ui;
}

QJsonObject FilterUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    FilterParametersKeys keys;
    parameters[keys.option] = ui->comboBoxOption->currentData().toInt();
    parameters[keys.format] = ui->comboBoxFormat->currentData().toInt();
    parameters[keys.escape] = ui->comboBoxEscape->currentData().toInt();
    parameters[keys.reference] = ui->lineEditReference->text();
    return parameters;
}

void FilterUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    FilterParametersKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();

    int defaultOption = static_cast<int>(ResponseOption::InputEqualReference);
    QVariant option = parametersMap.value(keys.option, defaultOption);
    QVariant format = parametersMap.value(keys.format, static_cast<int>(TextFormat::Ascii));
    QVariant escape = parametersMap.value(keys.escape, static_cast<int>(EscapeCharacter::None));
    QVariant reference = parametersMap.value(keys.reference, QString("(null)"));

    ui->comboBoxOption->setCurrentIndex(ui->comboBoxOption->findData(option));
    ui->comboBoxFormat->setCurrentIndex(ui->comboBoxFormat->findData(format));
    ui->comboBoxEscape->setCurrentIndex(ui->comboBoxEscape->findData(escape));
    ui->lineEditReference->setText(reference.toString());
}
