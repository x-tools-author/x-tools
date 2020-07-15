/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKSingletonErrorDialog.hh"
#include "ui_SAKSingletonErrorDialog.h"

SAKSingletonErrorDialog::SAKSingletonErrorDialog(QWidget *parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKSingletonErrorDialog)
{
    mUi->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

SAKSingletonErrorDialog::~SAKSingletonErrorDialog()
{
    delete mUi;
}
