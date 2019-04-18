/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef SAKHIGHLIGHTERSETTINGPANEL_HPP
#define SAKHIGHLIGHTERSETTINGPANEL_HPP

#include <QDialog>
#include <QTextDocument>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

namespace Ui {
class SAKHighlighterSettingPanel;
}

class SAKHighlighter;

class SAKHighlighterSettingPanel:public QDialog
{
    Q_OBJECT
public:
    SAKHighlighterSettingPanel(QTextDocument *doc, QWidget* parent = nullptr);
    ~SAKHighlighterSettingPanel();

    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::SAKHighlighterSettingPanel  *ui             = nullptr;
    SAKHighlighter                  *highlighter    = nullptr;
    QLineEdit                       *inputLineEdit  = nullptr;
    QPushButton                     *clearLabelBt   = nullptr;
    QPushButton                     *addLabelBt     = nullptr;

    QGridLayout                     labelLayout;
    QList<QPushButton*>             labelList;
private:    
    void clearLabel();    
    void resetLabelViewer();
    void addLabelFromInput();
    void addLabel(QString str);
    void deleteLabel(QPushButton *bt);
    void resetHighlightKeyword(QStringList keyWords);
};

#endif
