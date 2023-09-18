/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKPreferencesUi.hh"
#include "ui_SAKPreferencesUi.h"

SAKPreferencesUi::SAKPreferencesUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAKPreferencesUi)
{
    ui->setupUi(this);
}

SAKPreferencesUi::~SAKPreferencesUi()
{
    delete ui;
}
