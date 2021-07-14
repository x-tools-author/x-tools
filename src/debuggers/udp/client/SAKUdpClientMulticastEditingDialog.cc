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
    , mUi(new Ui::SAKUdpClientMulticastEditingDialog)
{
    mUi->setupUi(this);
    setModal(true);
}

SAKUdpClientMulticastEditingDialog::~SAKUdpClientMulticastEditingDialog()
{
    delete mUi;
}

QString SAKUdpClientMulticastEditingDialog::address()
{
    return mUi->addressLineEdit->text().trimmed();
}

quint16 SAKUdpClientMulticastEditingDialog::port()
{
    return mUi->portLineEdit->text().trimmed().toUShort();
}
