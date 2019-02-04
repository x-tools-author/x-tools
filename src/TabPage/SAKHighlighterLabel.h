/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKHIGHLIGHTERLABEL_H
#define SAKHIGHLIGHTERLABEL_H

#include <QWidget>

namespace Ui {
class SAKHighlighterLabel;
}

class SAKHighlighterLabel:public QWidget
{
    Q_OBJECT
public:
    SAKHighlighterLabel(QString label, QWidget* parent = nullptr);
    ~SAKHighlighterLabel();
private:
    Ui::SAKHighlighterLabel* ui;
};

#endif
