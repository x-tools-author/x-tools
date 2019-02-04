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

#include "SAKHighlighterLabel.h"
#include "ui_SAKHighlighterLabel.h"

SAKHighlighterLabel::SAKHighlighterLabel(QString label, QWidget* parent)
    :QWidget (parent)
    ,ui(new Ui::SAKHighlighterLabel)
{
    ui->setupUi(this);
    ui->label->setText(label);
}

SAKHighlighterLabel::~SAKHighlighterLabel()
{

}
