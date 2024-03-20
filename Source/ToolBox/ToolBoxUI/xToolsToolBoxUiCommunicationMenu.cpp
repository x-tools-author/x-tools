/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolBoxUiCommunicationMenu.h"
#include "ui_xToolsToolBoxUiCommunicationMenu.h"

#include <QWidgetAction>

#include "xToolsToolBox.h"

xToolsToolBoxUiCommunicationMenu::xToolsToolBoxUiCommunicationMenu(QWidget* parent)
    : QMenu(parent)
    , ui(new Ui::xToolsToolBoxUiCommunicationMenu)
{
    QWidget* w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);
}

xToolsToolBoxUiCommunicationMenu::~xToolsToolBoxUiCommunicationMenu()
{
    delete ui;
}

void xToolsToolBoxUiCommunicationMenu::initialize(xToolsToolBox* toolBox, const QString& settingsGroup)
{
    auto storer = toolBox->getStorerTool();
    auto storerGroup = settingsGroup + "/storer";

    ui->widgetStorer->initialize(storer, storerGroup);
}
