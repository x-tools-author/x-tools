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

#include "SAKMainWindowQrCode.hh"
#include "SAKMainWindowQrCodeView.hh"

#include "ui_SAKMainWindowQrCodeView.h"

SAKMainWindowQrCodeView::SAKMainWindowQrCodeView(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKMainWindowQrCodeView)
{
    ui->setupUi(this);
    setModal(true);
    resize(350, 580);

    tabWidget = ui->tabWidget;
    tabWidget->clear();

    addQRCode(tr("用户交流"), QString(":/resources/images/QSAKQQ.jpg"));
    addQRCode(tr("技术交流"), QString(":/resources/images/QtQQ.jpg"));
}

SAKMainWindowQrCodeView::~SAKMainWindowQrCodeView()
{
    delete ui;
}

void SAKMainWindowQrCodeView::addQRCode(QString name, QString image)
{
    tabWidget->addTab(new SAKMainWindowQrCode(size(), image, this), name);
}
