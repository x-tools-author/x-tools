/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "MoreInformation.h"
#include "ui_MoreInformation.h"

MoreInformation::MoreInformation(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::MoreInformation)
{
    ui->setupUi(this);
    setModal(true);
}
