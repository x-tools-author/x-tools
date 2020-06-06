/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKQRCodeCreator.hh"
#include "ui_SAKQRCodeCreator.h"
SAKQRCodeCreator::SAKQRCodeCreator(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKQRCodeCreator)
{
    ui->setupUi(this);
}

SAKQRCodeCreator::~SAKQRCodeCreator()
{
    delete ui;
}
