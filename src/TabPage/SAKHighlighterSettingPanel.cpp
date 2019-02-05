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
#include "SAKHighlighter.h"
#include "SAKHighlighterLabel.h"
#include "ui_SAKHighlighterSettingPanel.h"

#include <QDebug>

SAKHighlighterSettingPanel::SAKHighlighterSettingPanel(QTextDocument* doc, QWidget* parent)
    :QWidget (parent)
    ,ui(new Ui::SAKHighlighterSettingPanel)
{
    ui->setupUi(this);
    ui->frame->setLayout(&labelLayout);

    highlighter = new SAKHighlighter(doc);
    inputLineEdit = ui->lineEdit;
}

SAKHighlighterSettingPanel::~SAKHighlighterSettingPanel()
{

}

void SAKHighlighterSettingPanel::addLabel(QString str)
{
    if (str.isEmpty()){
        return;
    }

    QStringList keyWords;
    for (int i = 0; i < labelList.length(); i++){
        QString temp = labelList.at(i)->labelString();
        /// 标签重复不处理
        if (temp.compare(str) == 0){
            return;
        }
        keyWords.append(temp);
    }


    SAKHighlighterLabel* tempLabel = new SAKHighlighterLabel(str);
    labelList.append(tempLabel);
    connect(tempLabel, &SAKHighlighterLabel::deleteThis, this, &SAKHighlighterSettingPanel::deleteLabel);

    for (int index = 0; index < labelList.count(); index++){
        labelLayout.addWidget(labelList.at(index), index/5, index%5);
    }

    keyWords.append(str);
    highlighter->setHighlighterKeyWord(keyWords);
    highlighter->rehighlight();
}

void SAKHighlighterSettingPanel::deleteLabel(SAKHighlighterLabel*label)
{
    for (int i= 0; i < labelList.length(); i++){
        if (labelList.at(i) == label){
            SAKHighlighterLabel *temp = labelList.takeAt(i);
            temp->deleteLater();
            break;
        }
    }

    QStringList keyWords;
    for (int i = 0; i < labelList.length(); i++){
        keyWords.append(labelList.at(i)->labelString());
    }
    highlighter->setHighlighterKeyWord(keyWords);
}
