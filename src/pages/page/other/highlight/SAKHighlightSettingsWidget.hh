/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
