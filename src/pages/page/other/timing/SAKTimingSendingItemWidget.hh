/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
#include <QPushButton>
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
    SAKTimingSendingItemWidget(SAKDebugPage *debugPage,
                               quint64 id,
                               quint32 interval,
                               quint32 format,
                               QString comment,
                               QString data,
                               QWidget *parent = Q_NULLPTR);
    ~SAKTimingSendingItemWidget();

    quint64 parameterID();
    quint32 parameterInterval();
    quint32 parameterFormat();
    QString parameterComment();
    QString parameterData();
private:
    SAKDebugPage *debugPage;
    quint64 id;
    QTimer writeTimer;
private:
    void write();
    void initUi();
private:
    Ui::SAKTimingSendingItemWidget *ui;
    QCheckBox *timingCheckBox;
    QLineEdit *timingTimeLineEdit;
    QComboBox *textFormatComboBox;
    QLineEdit *remarkLineEdit;
    QTextEdit *inputDataTextEdit;
    QPushButton *updatePushButton;
private slots:
    void on_timingCheckBox_clicked();
    void on_timingTimeLineEdit_textChanged(const QString &text);
    void on_updatePushButton_clicked();
};

#endif
