/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKUdpClientToolBoxUiController.hh"
#include "ui_SAKUdpClientToolBoxUiController.h"

SAKUdpClientToolBoxUiController::SAKUdpClientToolBoxUiController(QWidget *parent)
    : SAKToolBoxUiController{parent}
    , ui(new Ui::SAKUdpClientToolBoxUiController)
{
    ui->setupUi(this);
}

SAKUdpClientToolBoxUiController::~SAKUdpClientToolBoxUiController()
{
    delete ui;
}
