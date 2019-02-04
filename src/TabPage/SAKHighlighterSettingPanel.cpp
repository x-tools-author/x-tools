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

#include "SAKHighlighterSettingPanel.h"
#include "ui_SAKHighlighterSettingPanel.h"

SAKHighlighterSettingPanel::SAKHighlighterSettingPanel(QWidget* parent)
    :QWidget (parent)
    ,ui(new Ui::SAKHighlighterSettingPanel)
{
    ui->setupUi(this);
}

SAKHighlighterSettingPanel::~SAKHighlighterSettingPanel()
{

}
