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
    ui->widgetTxMasker->setToolName("Tx masker");
    ui->widgetRxMasker->setToolName("Rx masker");
    ui->widgetTxAnanyzer->setToolName(tr("Tx analyzer"));
    ui->widgetRxAnanlyzer->setToolName(tr("Rx analyzer"));
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto flag = Qt::SkipEmptyParts;
#else
    auto flag = QString::SkipEmptyParts;
#endif
    QStringList txtList = txt.split(';', flag);
    ctx.output.keyWords = txtList;

    return ctx;
}

void SAKToolBoxUiParameters::initialize(SAKToolBox *toolBox,
                                        const QString &settingsGroup)
{
    auto txM = toolBox->getTxMaskerTool();
    auto rxM = toolBox->getRxMaskerTool();
    auto txA = toolBox->getTxAnalyzerTool();
    auto rxA = toolBox->getRxAnalyzerTool();
    auto storer = toolBox->getStorerTool();

    auto txMGroup = settingsGroup + "/txMasker";
    auto rxMGroup = settingsGroup + "/rxMasker";
    auto txAGroup = settingsGroup + "/txAnalyzer";
    auto rxAGroup = settingsGroup + "/rxAnalyzer";
    auto storerGroup = settingsGroup + "/storer";

    ui->widgetRxMasker->initialize(rxM, rxMGroup);
    ui->widgetTxMasker->initialize(txM, txMGroup);
    ui->widgetTxAnanyzer->initialize(txA, txAGroup);
    ui->widgetRxAnanlyzer->initialize(rxA, rxAGroup);
    ui->widgetStorer->initialize(storer, storerGroup);

    ui->checkBoxBigEndian->setGroupKey(settingsGroup + "/input", "bigEndian");
    ui->checkBoxCrcEnable->setGroupKey(settingsGroup + "/input", "crcEnable");
    ui->spinBoxEndIndex->setGroupKey(settingsGroup   + "/input", "endIndex");
    ui->spinBoxStartIndex->setGroupKey(settingsGroup + "/input", "startIndex");
    ui->comboBoxAlgorithm->setGroupKey(settingsGroup + "/input", "algorithm");
}
