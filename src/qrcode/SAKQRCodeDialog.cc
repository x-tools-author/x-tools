/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QImage>

#include "SAKQRCodeWidget.hh"
#include "SAKQRCodeDialog.hh"

#include "ui_SAKQRCodeDialog.h"

SAKQRCodeDialog::SAKQRCodeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKQRCodeDialog)
{
    ui->setupUi(this);
    setModal(true);
    resize(350, 580);

    tabWidget = ui->tabWidget;
    tabWidget->clear();

    addQRCode(tr("用户交流"), QString(":/resources/images/QSAKQQ.jpg"));
    addQRCode(tr("技术交流"), QString(":/resources/images/QtQQ.jpg"));
}

SAKQRCodeDialog::~SAKQRCodeDialog()
{
    delete ui;
}

void SAKQRCodeDialog::addQRCode(QString name, QString image)
{
    tabWidget->addTab(new SAKQRCodeWidget(size(), image, this), name);
}
