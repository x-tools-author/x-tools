/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
