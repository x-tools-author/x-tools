/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
