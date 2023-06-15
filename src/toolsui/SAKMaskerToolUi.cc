/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKMaskerTool.hh"
#include "SAKMaskerToolUi.hh"
#include "ui_SAKMaskerToolUi.h"

SAKMaskerToolUi::SAKMaskerToolUi(QWidget *parent)
    : SAKBaseToolUi(parent)
    , ui(new Ui::SAKMaskerToolUi)
{
    ui->setupUi(this);
}

SAKMaskerToolUi::~SAKMaskerToolUi()
{
    delete ui;
}

void SAKMaskerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}

void SAKMaskerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                              const QString &settingsGroup)
{
    ui->checkBoxEnable->setGroupKey(settingsGroup, "maskerEnable");
    ui->spinBoxMaskCode->setGroupKey(settingsGroup, "maskCode");

    SAKMaskerTool *cookedTool = qobject_cast<SAKMaskerTool*>(tool);
    if (!cookedTool) {
        qCWarning((*mLoggingCategory)) << "Invalid tool type!";
        return;
    }

    connect(ui->checkBoxEnable, &QCheckBox::clicked, this, [=](){
        cookedTool->setEnable(ui->checkBoxEnable->isChecked());
    });
    connect(ui->spinBoxMaskCode,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [=](int code){
        cookedTool->setMaskCode(code);
    });
}
