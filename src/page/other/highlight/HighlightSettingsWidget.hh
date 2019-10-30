/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef HIGHLIGHTERSETTINGSWIDGET_HH
#define HIGHLIGHTERSETTINGSWIDGET_HH

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTextDocument>

namespace Ui {
class HighlightSettingsWidget;
}

class HighlightSettings;

class HighlightSettingsWidget:public QWidget
{
    Q_OBJECT
public:
    HighlightSettingsWidget(QTextDocument *doc, QWidget* parent = nullptr);
    ~HighlightSettingsWidget();

    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::HighlightSettingsWidget     *ui             = nullptr;
    HighlightSettings               *highlighter    = nullptr;
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
