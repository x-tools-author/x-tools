/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTIMINGSENDINGITEMWIDGET_HH
#define SAKTIMINGSENDINGITEMWIDGET_HH

#include <QTimer>
#include <QRegExp>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRegExpValidator>

class SAKDebugPage;

namespace Ui {
    class SAKTimingSendingItemWidget;
}

class SAKTimingSendingItemWidget:public QWidget
{
    Q_OBJECT
public:
    SAKTimingSendingItemWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTimingSendingItemWidget();

private:
    Ui::SAKTimingSendingItemWidget *ui;

    QCheckBox *timingCheckBox;
    QLineEdit *timingTimeLineEdit;
    QComboBox *textFormatComboBox;
    QLineEdit *remarkLineEdit;
    QTextEdit *inputDataTextEdit;
private slots:
    void on_timingCheckBox_clicked();
    void on_timingTimeLineEdit_textChanged(const QString &text);

private:
    SAKDebugPage *debugPage;

private:
    QTimer writeTimer;
    void write();
};

#endif
