/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QApplication>
#include <QStyle>

#include "SAKHighlighterLabel.h"
#include "ui_SAKHighlighterLabel.h"

SAKHighlighterLabel::SAKHighlighterLabel(QString label, QWidget* parent)
    :QWidget (parent)
    ,ui(new Ui::SAKHighlighterLabel)
{
    ui->setupUi(this);
    ui->label->setText(label);

    ui->pushButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    connect(ui->pushButton, &QPushButton::clicked, this, &SAKHighlighterLabel::deleteLabel);
}

SAKHighlighterLabel::~SAKHighlighterLabel()
{
    delete  ui;
}

QString SAKHighlighterLabel::labelString()
{
    return ui->label->text();
}

void SAKHighlighterLabel::deleteLabel()
{
    emit deleteThis(this);
}
