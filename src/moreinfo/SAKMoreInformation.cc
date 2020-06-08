/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKMoreInformation.hh"
#include "ui_SAKMoreInformation.h"

#include <QFile>

SAKMoreInformation::SAKMoreInformation(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::SAKMoreInformation)
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
