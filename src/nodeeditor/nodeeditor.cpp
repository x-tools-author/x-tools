/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditor.h"
#include "ui_nodeeditor.h"

#include <QVBoxLayout>

#include "nodeeditornavigator.h"

namespace xFlow {

NodeEditor::NodeEditor(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::NodeEditor)
{
    ui->setupUi(this);
    ui->widgetNavigator->setupScrollArea(ui->scrollArea);
}

NodeEditor::~NodeEditor()
{
    delete ui;
}

} // namespace xFlow
