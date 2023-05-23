/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKResponserToolUiEditor.hh"
#include "ui_SAKResponserToolUiEditor.h"

SAKResponserToolUiEditor::SAKResponserToolUiEditor(QWidget *parent)
    : QDialog{parent}
{
    ui->setupUi(this);
}

SAKResponserToolUiEditor::~SAKResponserToolUiEditor()
{
    delete ui;
}

QJsonObject SAKResponserToolUiEditor::parameters()
{
    return QJsonObject();
}

void SAKResponserToolUiEditor::setParameters(const QJsonObject &params)
{
    Q_UNUSED(params)
}
