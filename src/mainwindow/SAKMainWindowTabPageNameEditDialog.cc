/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKMainWindowTabPageNameEditDialog.hh"
#include "ui_SAKMainWindowTabPageNameEditDialog.h"

SAKMainWindowTabPageNameEditDialog::SAKMainWindowTabPageNameEditDialog(QWidget *parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKMainWindowTabPageNameEditDialog)
{
    mUi->setupUi(this);
    mTabPageNameLineEdit = mUi->tabPageNameLineEdit;
    setModal(true);
}

SAKMainWindowTabPageNameEditDialog::~SAKMainWindowTabPageNameEditDialog()
{
    delete mUi;
}

void SAKMainWindowTabPageNameEditDialog::setName(const QString &name)
{
    mTabPageNameLineEdit->setText(name);
}

QString SAKMainWindowTabPageNameEditDialog::name()
{
    return mTabPageNameLineEdit->text().trimmed();
}
