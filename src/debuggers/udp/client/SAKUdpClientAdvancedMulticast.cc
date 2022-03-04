/*
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKUdpClientAdvancedMulticast.hh"
#include "ui_SAKUdpClientAdvancedMulticast.h"

SAKUdpClientAdvancedMulticast::SAKUdpClientAdvancedMulticast(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKUdpClientAdvancedMulticast)
{
    ui->setupUi(this);
    setModal(true);
}

SAKUdpClientAdvancedMulticast::~SAKUdpClientAdvancedMulticast()
{
    delete ui;
}

QString SAKUdpClientAdvancedMulticast::address()
{
    return ui->addressLineEdit->text().trimmed();
}

quint16 SAKUdpClientAdvancedMulticast::port()
{
    return ui->portLineEdit->text().trimmed().toInt();
}
