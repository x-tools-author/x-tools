/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakmaskertoolui.h"
#include "sakmaskertool.h"
#include "ui_sakmaskertoolui.h"

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

void SAKMaskerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup)
{
#if 0
    ui->checkBoxEnable->setGroupKey(settingsGroup, "maskerEnable");
#endif
    ui->spinBoxMaskCode->setGroupKey(settingsGroup, "maskCode");

    SAKMaskerTool *cookedTool = qobject_cast<SAKMaskerTool *>(tool);
    if (!cookedTool) {
        qCWarning((*mLoggingCategory)) << "Invalid tool type!";
        return;
    }

    connect(ui->checkBoxEnable, &QCheckBox::clicked, this, [=]() {
        cookedTool->setIsEnable(ui->checkBoxEnable->isChecked());
    });
    connect(ui->spinBoxMaskCode,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            [=](int code) { cookedTool->setMaskCode(code); });
    
    cookedTool->setIsEnable(ui->checkBoxEnable->isChecked());
    cookedTool->setMaskCode(ui->spinBoxMaskCode->value());
}
