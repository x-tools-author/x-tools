/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKHIGHLIGHTERSETTINGSWIDGET_HH
#define SAKHIGHLIGHTERSETTINGSWIDGET_HH

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTextDocument>

namespace Ui {
class SAKHighlightSettingsWidget;
}

class SAKHighlightSettings;

class SAKHighlightSettingsWidget:public QWidget
{
    Q_OBJECT
public:
    SAKHighlightSettingsWidget(QTextDocument *doc, QWidget* parent = Q_NULLPTR);
    ~SAKHighlightSettingsWidget();

    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::SAKHighlightSettingsWidget  *ui             = Q_NULLPTR;
    SAKHighlightSettings            *highlighter    = Q_NULLPTR;
    QLineEdit                       *inputLineEdit  = Q_NULLPTR;
    QPushButton                     *clearLabelBt   = Q_NULLPTR;
    QPushButton                     *addLabelBt     = Q_NULLPTR;

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
