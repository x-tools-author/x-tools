/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "searchpanel.h"
#include "ui_searchpanel.h"

SearchPanel::SearchPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::SearchPanel)
{
    ui->setupUi(this);
}

SearchPanel::~SearchPanel()
{
    delete ui;
}