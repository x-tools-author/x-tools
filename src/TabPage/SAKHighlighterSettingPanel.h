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
#include <QTextDocument>
#include <QLineEdit>
#include <QGridLayout>

namespace Ui {
class SAKHighlighterSettingPanel;
}

class SAKHighlighter;
class SAKHighlighterLabel;

class SAKHighlighterSettingPanel:public QWidget
{
    Q_OBJECT
public:
    SAKHighlighterSettingPanel(QTextDocument *doc, QWidget* parent = nullptr);
    ~SAKHighlighterSettingPanel();

    QLineEdit* inputLineEditInstance(){return inputLineEdit;}
    void addLabel(QString str);
private:
    Ui::SAKHighlighterSettingPanel* ui = nullptr;
    SAKHighlighter* highlighter = nullptr;
    QLineEdit* inputLineEdit = nullptr;

    QGridLayout labelLayout;
    QList<SAKHighlighterLabel*> labelList;
};

#endif
