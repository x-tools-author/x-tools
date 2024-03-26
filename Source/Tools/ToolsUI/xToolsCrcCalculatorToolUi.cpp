/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCrcCalculatorToolUi.h"
#include "ui_xToolsCrcCalculatorToolUi.h"

#include "xToolsCrcCalculatorTool.h"

xToolsCrcCalculatorToolUi::xToolsCrcCalculatorToolUi(QWidget *parent)
    : xToolsBaseToolUi(parent)
    , ui(new Ui::xToolsCrcCalculatorToolUi)
{
    ui->setupUi(this);
}

xToolsCrcCalculatorToolUi::~xToolsCrcCalculatorToolUi() {}

void xToolsCrcCalculatorToolUi::onIsWorkingChanged(bool isWorking)
{
    setEnabled(isWorking);
}

void xToolsCrcCalculatorToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)
    xToolsCrcCalculatorTool *crcTool = qobject_cast<xToolsCrcCalculatorTool *>(tool);
    if (!crcTool) {
        return;
    }

    ui->spinBoxStartIndex->setValue(crcTool->startIndex());
    ui->spinBoxEndIndex->setValue(crcTool->endIndex());
    ui->comboBoxAlgorithm->setCurrentIndex(crcTool->algorithm());
    ui->checkBoxBigEndian->setChecked(crcTool->bigEndian());
    connect(ui->spinBoxStartIndex,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            crcTool,
            &xToolsCrcCalculatorTool::setStartIndex);
    connect(ui->spinBoxEndIndex,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            crcTool,
            &xToolsCrcCalculatorTool::setEndIndex);
    connect(ui->comboBoxAlgorithm,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            crcTool,
            &xToolsCrcCalculatorTool::setAlgorithm);
    connect(ui->checkBoxBigEndian,
            &QCheckBox::toggled,
            crcTool,
            &xToolsCrcCalculatorTool::setBigEndian);
}
