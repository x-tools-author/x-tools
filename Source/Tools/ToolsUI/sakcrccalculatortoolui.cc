/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakcrccalculatortoolui.h"
#include "ui_sakcrccalculatortoolui.h"

#include "sakcrccalculatortool.h"

SAKCrcCalculatorToolUi::SAKCrcCalculatorToolUi(QWidget *parent)
    : SAKBaseToolUi(parent)
    , ui(new Ui::SAKCrcCalculatorToolUi)
{
    ui->setupUi(this);
}

SAKCrcCalculatorToolUi::~SAKCrcCalculatorToolUi() {}

void SAKCrcCalculatorToolUi::onIsWorkingChanged(bool isWorking)
{
    setEnabled(isWorking);
}

void SAKCrcCalculatorToolUi::onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)
    SAKCrcCalculatorTool *crcTool = qobject_cast<SAKCrcCalculatorTool *>(tool);
    if (!crcTool) {
        return;
    }

    ui->spinBoxStartIndex->setValue(crcTool->startIndex());
    ui->spinBoxEndIndex->setValue(crcTool->endIndex());
    ui->comboBoxAlgorithm->setCurrentIndex(crcTool->algorithm());
    ui->checkBoxBigEndian->setChecked(crcTool->bigEndian());
    connect(ui->spinBoxStartIndex,
            QOverload<int>::of(&QSpinBox::valueChanged),
            crcTool,
            &SAKCrcCalculatorTool::setStartIndex);
    connect(ui->spinBoxEndIndex,
            QOverload<int>::of(&QSpinBox::valueChanged),
            crcTool,
            &SAKCrcCalculatorTool::setEndIndex);
    connect(ui->comboBoxAlgorithm,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            crcTool,
            &SAKCrcCalculatorTool::setAlgorithm);
    connect(ui->checkBoxBigEndian,
            &QCheckBox::toggled,
            crcTool,
            &SAKCrcCalculatorTool::setBigEndian);
}