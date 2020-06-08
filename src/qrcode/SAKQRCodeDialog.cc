/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
