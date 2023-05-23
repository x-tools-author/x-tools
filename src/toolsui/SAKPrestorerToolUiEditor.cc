/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKPrestorerToolUiEditor.hh"
#include "ui_SAKPrestorerToolUiEditor.h"

SAKPrestorerToolUiEditor::SAKPrestorerToolUiEditor(QWidget *parent)
    : QDialog{parent}
{
    ui->setupUi(this);
}

SAKPrestorerToolUiEditor::~SAKPrestorerToolUiEditor()
{
    delete ui;
}

QJsonObject SAKPrestorerToolUiEditor::parameters()
{
    return QJsonObject();
}

void SAKPrestorerToolUiEditor::setParameters(const QJsonObject &params)
{
    Q_UNUSED(params)
}
