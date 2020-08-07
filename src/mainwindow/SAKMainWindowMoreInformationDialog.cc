/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKMainWindowMoreInformationDialog.hh"
#include "ui_SAKMainWindowMoreInformationDialog.h"

#include <QFile>

SAKMainWindowMoreInformationDialog::SAKMainWindowMoreInformationDialog(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::SAKMainWindowMoreInformationDialog)
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
