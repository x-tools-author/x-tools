/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKEmitterToolUiEditor.hh"
#include "ui_SAKEmitterToolUiEditor.h"

SAKEmitterToolUiEditor::SAKEmitterToolUiEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::SAKEmitterToolUiEditor)
{
    ui->setupUi(this);
    setModal(true);

    connect(ui->pushButtonApply, &QPushButton::clicked,
            this, &SAKEmitterToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &SAKEmitterToolUiEditor::reject);
}

SAKEmitterToolUiEditor::~SAKEmitterToolUiEditor()
{
    delete ui;
}

QJsonObject SAKEmitterToolUiEditor::parameters()
{
    return QJsonObject();
}

void SAKEmitterToolUiEditor::setParameters(const QJsonObject &params)
{

}
