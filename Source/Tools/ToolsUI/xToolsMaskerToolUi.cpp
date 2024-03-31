/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsMaskerToolUi.h"
#include "ui_xToolsMaskerToolUi.h"

#include <QDebug>

#include "xToolsMaskerTool.h"

xToolsMaskerToolUi::xToolsMaskerToolUi(QWidget *parent)
    : xToolsBaseToolUi(parent)
    , ui(new Ui::xToolsMaskerToolUi)
{
    ui->setupUi(this);
}

xToolsMaskerToolUi::~xToolsMaskerToolUi()
{
    delete ui;
}

void xToolsMaskerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}

void xToolsMaskerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
#if 0
    ui->checkBoxEnable->setGroupKey(settingsGroup, "maskerEnable");
#endif
    ui->spinBoxMaskCode->setGroupKey(settingsGroup, "maskCode");

    xToolsMaskerTool *cookedTool = qobject_cast<xToolsMaskerTool *>(tool);
    if (!cookedTool) {
        qWarning() << "Invalid tool type!";
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
