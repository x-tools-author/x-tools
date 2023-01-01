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
#include "SAKToolAsciiAssistant.hh"
#include "ui_SAKToolAsciiAssistant.h"

SAKToolAsciiAssistant::SAKToolAsciiAssistant(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKToolAsciiAssistant)
{
    ui->setupUi(this);
    const QPixmap pixmap = QPixmap::fromImage(QImage(":/resources/ASCII.png"));
    ui->labelImage->setPixmap(pixmap);
}

SAKToolAsciiAssistant::~SAKToolAsciiAssistant()
{
    delete ui;
}
