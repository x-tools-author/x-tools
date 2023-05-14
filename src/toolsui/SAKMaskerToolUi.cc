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
    : QWidget(parent)
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

void SAKMaskerToolUi::setupMasker(SAKMaskerTool *tool)
{
    if (!tool) {
        return;
    }

    if (mTool) {
        disconnect(this, nullptr,
                   const_cast<const SAKMaskerTool*>(mTool), nullptr);
    }

    mTool = tool;
    connect(ui->checkBoxEnable, &QCheckBox::clicked, this, [=](){
        mTool->setProperty("enable", ui->checkBoxEnable->isChecked());
    });
    connect(ui->spinBoxMaskCode, &QSpinBox::valueChanged, this, [=](int code){
        mTool->setMaskCode(code);
    });
}
