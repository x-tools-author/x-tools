/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "bdspanel.h"
#include "ui_bdspanel.h"

BdsPanel::BdsPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::BdsPanel)
{
    ui->setupUi(this);
}

BdsPanel::~BdsPanel()
{
    delete ui;
}