/***************************************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakasciiassistant.h"
#include "ui_sakasciiassistant.h"

#include <QImage>

SAKAsciiAssistant::SAKAsciiAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SAKAsciiAssistant)
{
    ui->setupUi(this);

    const QPixmap pixmap = QPixmap::fromImage(QImage(":/resources/ASCII.png"));
    resize(pixmap.size());
    ui->image_->setPixmap(pixmap);
}

SAKAsciiAssistant::~SAKAsciiAssistant()
{
    delete ui;
}
