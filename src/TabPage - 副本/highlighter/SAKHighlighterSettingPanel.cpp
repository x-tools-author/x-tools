/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SAKHighlighterSettingPanel.hpp"
#include "SAKHighlighter.hpp"
#include "ui_SAKHighlighterSettingPanel.h"

#include <QDebug>

SAKHighlighterSettingPanel::SAKHighlighterSettingPanel(QTextDocument* doc, QWidget* parent)
    :QDialog (parent)
    ,ui (new Ui::SAKHighlighterSettingPanel)
{
    ui->setupUi(this);
    ui->frame->setLayout(&labelLayout);

    highlighter = new SAKHighlighter(doc);
    inputLineEdit = ui->lineEdit;
    clearLabelBt = ui->pushButtonClear;
    addLabelBt = ui->pushButtonAdd;

    inputLineEdit->installEventFilter(this);

    connect(clearLabelBt, &QPushButton::clicked, this, &SAKHighlighterSettingPanel::clearLabel);
    connect(addLabelBt, &QPushButton::clicked, this, &SAKHighlighterSettingPanel::addLabelFromInput);

    setModal(true);
}

SAKHighlighterSettingPanel::~SAKHighlighterSettingPanel()
{
    delete ui;
}

void SAKHighlighterSettingPanel::addLabel(QString str)
{
    if (str.isEmpty()){
        return;
    }

    QStringList keyWords;
    for (int i = 0; i < labelList.length(); i++){
        QString temp = labelList.at(i)->text();
        /// 标签重复不处理
        if (temp.compare(str) == 0){
            return;
        }
        keyWords.append(temp);
    }


    QPushButton* tempLabel = new QPushButton(str);
    tempLabel->installEventFilter(this);
    labelList.append(tempLabel);

    resetLabelViewer();

    keyWords.append(str);
    resetHighlightKeyword(keyWords);
}

void SAKHighlighterSettingPanel::addLabelFromInput()
{
    QString str = inputLineEdit->text();
    addLabel(str);
}

void SAKHighlighterSettingPanel::deleteLabel(QPushButton *bt)
{
    for (int i= 0; i < labelList.length(); i++){
        if (labelList.at(i) == bt){
            QPushButton *BtTemp = labelList.takeAt(i);
            BtTemp->deleteLater();
            break;
        }
    }

    resetLabelViewer();

    QStringList keyWords;
    for (int i = 0; i < labelList.length(); i++){
        keyWords.append(labelList.at(i)->text());
    }    

    resetHighlightKeyword(keyWords);
}

bool SAKHighlighterSettingPanel::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick){
        for (int i = 0; i < labelList.length(); i++){
            if (labelList.at(i) == watched){
                deleteLabel(labelList.at(i));
                return true;
            }
        }
    }

    if (event->type() == QEvent::KeyPress){
        QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);
        if ((e->key() == Qt::Key_Enter) || e->key() == Qt::Key_Return){
            addLabel(inputLineEdit->text());
            inputLineEdit->clear();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void SAKHighlighterSettingPanel::clearLabel()
{
    QPushButton *bt = nullptr;
    while (!labelList.isEmpty()) {
        bt = labelList.takeFirst();
        bt->deleteLater();
    }

    resetHighlightKeyword(QStringList());
}

void SAKHighlighterSettingPanel::resetLabelViewer()
{
    for (int index = 0; index < labelList.count(); index++){
        labelLayout.addWidget(labelList.at(index), index/5, index%5);
    }
}

void SAKHighlighterSettingPanel::resetHighlightKeyword(QStringList keyWords)
{
    highlighter->setHighlighterKeyWord(keyWords);
    highlighter->rehighlight();
}
