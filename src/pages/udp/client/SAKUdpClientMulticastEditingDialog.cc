/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKUdpClientMulticastEditingDialog.hh"
#include "ui_SAKUdpClientMulticastEditingDialog.h"

SAKUdpClientMulticastEditingDialog::SAKUdpClientMulticastEditingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKUdpClientMulticastEditingDialog)
{
    ui->setupUi(this);
    setModal(true);
}

SAKUdpClientMulticastEditingDialog::~SAKUdpClientMulticastEditingDialog()
{
    delete ui;
}

QString SAKUdpClientMulticastEditingDialog::address()
{
    return ui->addressLineEdit->text().trimmed();
}

quint16 SAKUdpClientMulticastEditingDialog::port()
{
    return ui->portLineEdit->text().trimmed().toUShort();
}
