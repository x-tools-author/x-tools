/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolboxuicommunicationmenu.h"

#include <QWidgetAction>

#include "saktoolbox.h"
#include "ui_saktoolboxuicommunicationmenu.h"

SAKToolBoxUiCommunicationMenu::SAKToolBoxUiCommunicationMenu(QWidget* parent)
    : QMenu(parent)
    , ui(new Ui::SAKToolBoxUiCommunicationMenu)
{
    QWidget* w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);
}

SAKToolBoxUiCommunicationMenu::~SAKToolBoxUiCommunicationMenu()
{
    delete ui;
}

void SAKToolBoxUiCommunicationMenu::initialize(SAKToolBox* toolBox, const QString& settingsGroup)
{
    auto storer = toolBox->getStorerTool();
    auto storerGroup = settingsGroup + "/storer";

    ui->widgetStorer->initialize(storer, storerGroup);
}
