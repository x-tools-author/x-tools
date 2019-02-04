/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKHIGHLIGHTERSETTINGPANEL_H
#define SAKHIGHLIGHTERSETTINGPANEL_H

#include <QWidget>

namespace Ui {
class SAKHighlighterSettingPanel;
}

class SAKHighlighterSettingPanel:public QWidget
{
    Q_OBJECT
public:
    SAKHighlighterSettingPanel(QWidget* parent = nullptr);
    ~SAKHighlighterSettingPanel();
private:
    Ui::SAKHighlighterSettingPanel* ui = nullptr;
};

#endif
