/*
 * Copyright 2022 Qter(qsaker@qq.com. All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */

#include <QImage>
#include "QsakAsciiAssistant.hh"
#include "ui_QsakAsciiAssistant.h"

QsakAsciiAssistant::QsakAsciiAssistant(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QsakAsciiAssistant)
{
    ui->setupUi(this);
    const QPixmap pixmap = QPixmap::fromImage(QImage(":/resources/ASCII.png"));
    ui->labelImage->setPixmap(pixmap);
}

QsakAsciiAssistant::~QsakAsciiAssistant()
{
    delete ui;
}
