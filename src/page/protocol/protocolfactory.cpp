/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "protocolfactory.h"
#include "ui_protocolfactory.h"

ProtocolFactory::ProtocolFactory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProtocolFactory)
{
    ui->setupUi(this);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 10);
}

ProtocolFactory::~ProtocolFactory()
{
    delete ui;
}