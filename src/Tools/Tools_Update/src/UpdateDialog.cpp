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

#include "UpdateDialog.h"
#include "httpwindow.h"
#include "ui_UpdateDialog.h"

#include <QProgressBar>

UpdateDialog::UpdateDialog(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::UpdateDialog)
    ,httpWindow(new HttpWindow)
{
    ui->setupUi(this);
    setModal(true);

    //ui->pushButtonOK->hide();
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(ui->pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(updateSoftware()));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::checkForUpdate()
{
    show();
}

void UpdateDialog::cancel()
{
    close();

}

void UpdateDialog::updateSoftware()
{
    close();
    httpWindow->show();
}

void UpdateDialog::updateProgressBar()
{

}
