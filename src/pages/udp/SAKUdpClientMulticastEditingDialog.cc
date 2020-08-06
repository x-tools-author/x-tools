/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKUdpMulticastEditingDialog.hh"
#include "ui_SAKUdpMulticastEditingDialog.h"

SAKUdpMulticastEditingDialog::SAKUdpMulticastEditingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKUdpMulticastEditingDialog)
{
    ui->setupUi(this);
    setModal(true);
}

SAKUdpMulticastEditingDialog::~SAKUdpMulticastEditingDialog()
{
    delete ui;
}

QString SAKUdpMulticastEditingDialog::address()
{
    return ui->addressLineEdit->text().trimmed();
}

quint16 SAKUdpMulticastEditingDialog::port()
{
    return ui->portLineEdit->text().trimmed().toUShort();
}
