/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolBox.hh"
#include "SAKToolBoxUiParameters.hh"
#include "ui_SAKToolBoxUiParameters.h"

SAKToolBoxUiParameters::SAKToolBoxUiParameters(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKToolBoxUiParameters)
{
    ui->setupUi(this);
    ui->widgetInputMasker->setToolName("Input masker");
    ui->widgetOutputMasker->setToolName("Output masker");
    ui->widgetInputAnanyzer->setToolName(tr("Input analyzer"));
    ui->widgetOutputAnanlyzer->setToolName(tr("Output analyzer"));
}

SAKToolBoxUiParameters::~SAKToolBoxUiParameters()
{
    delete ui;
}

void SAKToolBoxUiParameters::showDialog(int tabIndex)
{
    if (tabIndex >= 0 && tabIndex < ui->tabWidget->count()) {
        ui->tabWidget->setCurrentIndex(tabIndex);
    } else {
        qCWarning(mLoggingCategory) << "Invalid tab index:" << tabIndex;
    }

    show();
}

SAKToolBoxUiParameters::ParameterContext
SAKToolBoxUiParameters::parameterContext()
{
    ParameterContext ctx{};

    ctx.input.preprocessing.prefix = ui->comboBoxPrefix->currentData().toInt();
    ctx.input.preprocessing.suffix = ui->comboBoxSuffix->currentData().toInt();
    ctx.input.preprocessing.escapeCharacter =
        ui->comboBoxEscapeCharacter->currentData().toInt();

    ctx.input.crc.enable = ui->checkBoxCrcEnable->isChecked();
    ctx.input.crc.bigEndian = ui->checkBoxBigEndian->isChecked();
    ctx.input.crc.arithmetic = ui->comboBoxAlgorithm->currentData().toInt();
    ctx.input.crc.startIndex = ui->spinBoxStartIndex->value();
    ctx.input.crc.endIndex = ui->spinBoxEndIndex->value();

    QString txt = ui->textEdit->toPlainText();
    QStringList txtList = txt.split(';', Qt::SkipEmptyParts);
    ctx.output.keyWords = txtList;

    return ctx;
}

void SAKToolBoxUiParameters::initialize(SAKToolBox *toolBox,
                                        const QString &settingsGroup)
{
    ui->checkBoxBigEndian->setGroupKey(settingsGroup, "bigEndian");
    ui->checkBoxCrcEnable->setGroupKey(settingsGroup, "crcEnable");
    ui->spinBoxEndIndex->setGroupKey(settingsGroup, "endIndex");
    ui->spinBoxStartIndex->setGroupKey(settingsGroup, "startIndex");

    ui->widgetInputMasker->initialize(toolBox->getInputMaskerTool(),
                                      settingsGroup + "/inputMasker");
    ui->widgetInputMasker->initialize(toolBox->getOutputMaskerTool(),
                                      settingsGroup + "/outputMasker");
    ui->widgetInputAnanyzer->initialize(toolBox->getInputAnalyzerTool(),
                                        settingsGroup + "/inputAnalyzer");
    ui->widgetOutputAnanlyzer->initialize(toolBox->getOutputAnalyzerTool(),
                                          settingsGroup + "/outputAnalyzer");
    ui->widgetStorer->initialize(toolBox->getStorerTool(),
                                 settingsGroup + "/storer");
}
