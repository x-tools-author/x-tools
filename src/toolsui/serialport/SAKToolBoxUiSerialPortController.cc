/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolBoxUiSerialPortController.hh"
#include "ui_SAKToolBoxUiSerialPortController.h"

SAKToolBoxUiSerialPortController::SAKToolBoxUiSerialPortController(
    SAKCommunicationTool *tool, QWidget *parent)
    : SAKToolBoxUiBaseController{tool, parent}
    , ui(new Ui::SAKToolBoxUiSerialPortController)
{
    ui->setupUi(this);
}

SAKToolBoxUiSerialPortController::~SAKToolBoxUiSerialPortController()
{
    delete ui;
}
