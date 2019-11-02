/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "ConfirmUpdateDialog.h"
#include "ui_ConfirmUpdateDialog.h"


ConfirmUpdateDialog::ConfirmUpdateDialog()
    :ui(new Ui::ConfirmUpdateDialog)
{
    ui->setupUi(this);

    newVersion = ui->labelUpdateVersion;
    oldVersion = ui->labelCurrentVersion;
    desc       = ui->labelDescription;

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
    setModal(true);

    connect(this, &ConfirmUpdateDialog::accepted, this, &ConfirmUpdateDialog::need2download);
}

ConfirmUpdateDialog::~ConfirmUpdateDialog()
{

}

void ConfirmUpdateDialog::setInfo(QString version, QUrl url, QString description)
{
    newVersion->setText(version);
    address = url;
    desc->setText(description);
}
