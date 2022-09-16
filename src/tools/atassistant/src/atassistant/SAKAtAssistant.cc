/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKAtAssistant.hh"
#include "ui_SAKAtAssistant.h"

SAKAtAssistant::SAKAtAssistant(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::SAKAtAssistant)
{
    ui_->setupUi(this);
}

SAKAtAssistant::~SAKAtAssistant()
{
    delete ui_;
}
