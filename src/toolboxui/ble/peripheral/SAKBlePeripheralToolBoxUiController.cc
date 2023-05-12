/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKBlePeripheralToolBoxUiController.hh"
#include "ui_SAKBlePeripheralToolBoxUiController.h"

SAKBlePeripheralToolBoxUiController::SAKBlePeripheralToolBoxUiController(QWidget *parent)
    : SAKToolBoxUiController{parent}
    , ui(new Ui::SAKBlePeripheralToolBoxUiController)
{
    ui->setupUi(this);
}

SAKBlePeripheralToolBoxUiController::~SAKBlePeripheralToolBoxUiController()
{
    delete ui;
}
