/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#include "MoreInformation.hh"
#include "ui_MoreInformation.h"

#include <QFile>

MoreInformation::MoreInformation(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::MoreInformation)
{
    ui->setupUi(this);
    setModal(true);

    textBrowserHistory = ui->textBrowserHistory;
    textBrowserHistory->clear();
    QFile file(":/resources/files/History.txt");
    if (file.open(QFile::ReadOnly)){
        QByteArray data = file.readAll();
        textBrowserHistory->setText(QString::fromUtf8(data));
    }
}
