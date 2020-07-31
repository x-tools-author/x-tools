/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
